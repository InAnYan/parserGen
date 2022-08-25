from abc import ABC, abstractmethod
from src.Tokens import Token
from src import GlobalArgs
from . import Style


class Job(ABC):
    parent: str
    save_res: bool
    save_type: str
    save_name: str

    def __init__(self, parent):
        self.save_res = False
        self.parent = parent

    def save_res_as(self, save_type, save_name):
        self.save_res = True
        self.save_type = save_type
        self.save_name = Style.temp(save_name)

    @abstractmethod
    def generate(self, global_decls):
        pass

    @abstractmethod
    def get_temps(self) -> [str]:
        pass

    @abstractmethod
    def get_tokens(self, global_decls, pos: int = 0) -> [Token]:
        pass


class TokenJob(Job):
    def __init__(self, parent, token):
        super().__init__(parent)
        self.token = token

    def get_temps(self) -> [str]:
        if self.save_res:
            return [Style.token() + ' ' + self.save_name]
        else:
            return []

    def generate(self, global_decls):
        if self.save_res:
            if self.save_type == 'container':
                Style.write(self.save_name + '.push_back(' + Style.require(self.token) + ');')
            else:
                Style.write(self.save_name + ' = ' + Style.require(self.token) + ';')
        else:
            Style.write(Style.require(self.token) + ';')

    def get_tokens(self, global_decls, pos=0) -> [Token]:
        if pos != 0:
            return []
        else:
            return [self.token]


class MethodJob(Job):
    def __init__(self, parent, method_name):
        super().__init__(parent)
        self.method_name = method_name

    def get_temps(self) -> [str]:
        if self.save_res and self.save_type is not None:
            return [Style.node_type(self.method_name) + ' ' + self.save_name]
        else:
            return []

    def generate(self, global_decls):
        if self.save_res:
            if self.save_type is None:
                Style.write(self.save_name + ' = ' + Style.parse(self.method_name) + '();')
            elif self.save_type == 'container':
                Style.write(self.save_name + '.push_back(' + Style.parse(self.method_name) + '()' + ');')
            else:
                Style.write(self.save_name + ' = ' +
                            Style.parse(self.method_name) + '();')
        else:
            Style.write(Style.parse(self.method_name) + '();')

    def get_tokens(self, global_decls, pos=0) -> [Token]:
        if self.method_name == self.parent:
            return []
        else:
            return global_decls[self.method_name].get_tokens(global_decls, pos)


class ManyJob(Job):
    def __init__(self, parent, job, can_be_zero):
        super().__init__(parent)
        self.job = job
        self.can_be_zero = can_be_zero

    def get_temps(self) -> [str]:
        if self.save_res:
            if type(self.job) is TokenJob:
                return [Style.container_type(Style.token()) + ' ' + self.save_name]
            elif type(self.job) is MethodJob:
                return [Style.container_type(Style.node_type(self.job.method_name)) + ' ' + self.save_name]
            else:
                print('ERROR: Unknown many job')
                exit(1)
        else:
            pass

    def save_res_as(self, save_type, save_name):
        super().save_res_as(save_type, save_name)
        self.job.save_res_as('container', save_name)

    def generate(self, global_decls):
        self_index = None
        job_list = global_decls[self.parent].jobs
        for i in range(len(job_list)):
            if job_list[i] == self:
                self_index = i
                break

        conditions = ''
        if self_index is not None:
            if self_index != len(job_list) - 1 and type(job_list[self_index + 1]) is TokenJob \
                    and job_list[self_index + 1].save_res is False and self.can_be_zero:
                conditions = '!' + Style.matching(job_list[self_index + 1].token)
                global_decls[self.parent].jobs.pop(self_index + 1)
            else:
                for i in range(len(self.job.get_tokens(global_decls))):
                    conditions += Style.current_token() + '.type == ' + Style.token_type(
                        self.job.get_tokens(global_decls)[i])
                    if i != len(self.job.get_tokens(global_decls)) - 1:
                        conditions += ' || '

        if self.can_be_zero:
            Style.write('while (' + conditions + ')')
        else:
            Style.write('do')

        Style.start_block()

        self.job.generate(global_decls)

        Style.end_block()

        if not self.can_be_zero:
            Style.write('while (' + conditions + ');')

    def get_tokens(self, global_decls, pos=0) -> [Token]:
        return self.job.get_tokens(global_decls, pos)


class VariantJob(Job):
    def __init__(self, parent, variants):
        super().__init__(parent)
        self.variants = variants

    def get_temps(self) -> [str]:
        res = []
        for variant in self.variants:
            for job in variant:
                for e in job.get_temps():
                    res.append(e)
        return res

    def get_tokens(self, global_decls, pos: int = 0) -> [Token]:
        res = []
        for variant in self.variants:
            res += variant[0].get_tokens(global_decls, pos)
        return res

    def generate(self, global_decls):
        for i in range(len(self.variants)):
            if i == len(self.variants) - 1:
                Style.write('else')
                Style.start_block()
                for e in self.variants[i]:
                    e.generate(global_decls)
                Style.end_block()
                break

            if_str = 'if (' if i == 0 else 'else if ('

            if len(self.variants[i]) == 1 and isinstance(self.variants[i][0], MethodJob) \
                    and not global_decls[self.variants[i][0].method_name].jobs[0].save_res \
                    and isinstance(global_decls[self.variants[i][0].method_name].jobs[0], TokenJob) \
                    and GlobalArgs.fast_j_cm():
                first_tok_to_match = global_decls[self.variants[i][0].method_name].jobs[0]
                global_decls[self.variants[i][0].method_name].jobs.pop(0)
                if_str += Style.matching(first_tok_to_match.token)
            else:
                first_toks = self.variants[i][0].get_tokens(global_decls)
                for j in range(len(first_toks)):
                    if_str += Style.current_token_equals_type(first_toks[j])
                    if j != len(first_toks) - 1:
                        if_str += ' || '

            Style.write(if_str + ')')
            Style.start_block()

            for e in self.variants[i]:
                e.generate(global_decls)

            Style.end_block()


class OptionalJob(Job):
    def __init__(self, parent: str, jobs: [Job]):
        super().__init__(parent)
        self.jobs = jobs

    def get_temps(self) -> [str]:
        res = []
        for job in self.jobs:
            for e in job.get_temps():
                res.append(e)
        return res

    def get_tokens(self, global_decls, pos: int = 0) -> [Token]:
        res = []
        for job in self.jobs:
            for e in job.get_tokens(global_decls, pos):
                res.append(e)
        return res

    def generate(self, global_decls):
        if type(self.jobs[0]) is TokenJob and self.jobs[0].save_res == False:
            Style.write('if (' + Style.matching(self.jobs[0].token) + ')')
            self.jobs.pop(0)
        else:
            conditions = ''
            first_toks = self.jobs[0].get_tokens(global_decls)
            for i in range(len(first_toks)):
                conditions += Style.current_token_equals_type(first_toks[i])
                if i != len(first_toks) - 1:
                    conditions += ' || '

            Style.write('if (' + conditions + ')')

        Style.start_block()
        for job in self.jobs:
            job.generate(global_decls)
        Style.end_block()


class ExprLeftAsoc(Job):
    def __init__(self, parent: str, job: str, operators: [str]):
        super().__init__(parent)
        self.job = job
        self.operators = []
        for e in operators:
            self.operators.append(Style.token_type(e))
        new_operators = ""
        for e in self.operators:
            if e != self.operators[-1]:
                new_operators += e + ', '
            else:
                new_operators += e
        self.operators = new_operators

    def generate(self, global_decls):
        Style.write(Style.abstract_node_type() + ' left = ' + Style.parse(self.job) + '();')
        Style.write('')
        Style.write('while (' + Style.matching_args(self.operators) + ')') # TODO: Check for end
        Style.start_block()
        Style.write(Style.token_ref() + ' op = ' + Style.previous_token() + ';')
        Style.write(Style.abstract_node_type() + ' right = ' + Style.parse(self.job) + '();')
        Style.write('left = std::make_unique<' + Style.node(self.parent) +
                    '>(std::move(left), op, std::move(right);')
        Style.end_block()
        Style.write('')
        Style.write('return left;')

    def get_temps(self) -> [str]:
        return []

    def get_tokens(self, global_decls, pos: int = 0) -> [Token]:
        return []


class ExprRightAsoc(Job):
    def __init__(self, parent: str, job: str, operators: [str]):
        super().__init__(parent)
        self.job = job
        self.operators = []
        for e in operators:
            self.operators.append(Style.token_type(e))
        new_operators = ""
        for e in self.operators:
            if e != self.operators[-1]:
                new_operators += e + ', '
            else:
                new_operators += e
        self.operators = new_operators

    def generate(self, global_decls):
        Style.write(Style.abstract_node_type() + ' left = ' + Style.parse(self.job) + '();')
        Style.write('')
        Style.write('if (' + Style.matching_args(self.operators) + ')')
        Style.start_block()
        Style.write(Style.token_ref() + ' op = ' + Style.previous_token() + ';')
        Style.write(Style.abstract_node_type() + ' right = ' + Style.parse(self.parent) + '();')
        Style.write('return std::make_unique<' + Style.node(self.parent) + '>(std::move(left), op, std::move(right));')
        Style.end_block()
        Style.write('')
        Style.write('return left;')

    def get_temps(self) -> [str]:
        return []

    def get_tokens(self, global_decls, pos: int = 0) -> [Token]:
        return []


class SelfReturnJob(Job):
    def __init__(self, parent: str, jobs: [Job]):
        super().__init__(parent)
        self.jobs = jobs

    def get_temps(self) -> [str]:
        return []

    def get_tokens(self, global_decls, pos: int = 0) -> [Token]:
        return []

    def generate(self, global_decls):
        pass