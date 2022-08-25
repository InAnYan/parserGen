from src.Jobs import Job, ExprLeftAsoc, ExprRightAsoc
from . import Style


jobsList = {}


class Declaration:
    name: str
    jobs: [Job]
    to_args: [str]
    self_return: bool

    def __init__(self, name: str, jobs: [Job], to_args: [str], self_return: bool = False):
        self.name = name
        self.jobs = jobs
        self.to_args = to_args
        self.self_return = self_return
        global jobsList
        jobsList[name] = self

    def generate(self):
        global jobsList
        if self.self_return:
            Style.write(Style.node_type(self.name) + ' ' + Style.temp(self.name) + ' = nullptr;')
            Style.write('')

        temps = []
        for job in self.jobs:
            for e in job.get_temps():
                temps.append(e)

        for e in temps:
            Style.write(e + ';')
        if temps:
            Style.write('')

        for job in self.jobs:
            job.generate(jobsList)

        if len(self.jobs) == 1 and (isinstance(self.jobs[0], ExprLeftAsoc) or isinstance(self.jobs[0], ExprRightAsoc)):
            return

        Style.write('')

        if self.self_return:
            Style.write('return std::move(' + Style.temp(self.name) + ');')
        else:
            Style.write('return std::make_unique<' + Style.node(self.name) + '>(')
            Style.tabLevel += 1
            for i in range(len(self.to_args)):
                to_write = 'std::move(' + Style.temp(self.to_args[i]) + ')'
                if i != len(self.to_args) - 1:
                    to_write += ', '
                Style.write(to_write)
            Style.tabLevel -= 1
            Style.write(');')

    def get_tokens(self, global_decls, pos=0):
        global jobsList
        return self.jobs[0].get_tokens(jobsList, pos)
