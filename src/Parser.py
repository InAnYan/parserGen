from src.Declaration import Declaration
from src.Tokens import TokenType, Token
from src.Jobs import *


class Parser:
    parser_pos: int
    token_list: [Token]
    current_method: str

    def __init__(self, token_list):
        self.parser_pos = 0
        self.token_list = token_list
        self.current_method = ''

    def match(self, token_type: TokenType) -> Token | None:
        if self.token_list[self.parser_pos].token_type == token_type:
            res = self.token_list[self.parser_pos]
            self.parser_pos += 1
            return res
        else:
            return None

    def require(self, token_type: TokenType) -> Token:
        matched = self.match(token_type)
        if matched:
            return matched
        else:
            print('ERROR: Expected ' + str(token_type.value))
            exit(1)

    def minor_parse(self, parser_jobs_list: [Job]) -> Job:
        cur_tok = self.token_list[self.parser_pos]
        self.parser_pos += 1
        cur_type = cur_tok.token_type

        if cur_type == TokenType.TOKEN:
            return TokenJob(self.current_method, cur_tok.text)
        elif cur_type == TokenType.METHOD:
            return MethodJob(self.current_method, cur_tok.text)
        elif cur_type == TokenType.PLUS or cur_type == TokenType.ASTERISK:
            temp = parser_jobs_list[-1]
            parser_jobs_list.pop()
            return ManyJob(self.current_method, temp, False if cur_type == TokenType.PLUS else True)
        elif cur_type == TokenType.SELF_RETURN:
            temp = parser_jobs_list[-1]
            parser_jobs_list.pop()
            temp.save_res_as(None, self.current_method)
            return temp
        elif cur_type == TokenType.OBRACKET:
            jobs = []
            while self.match(TokenType.CBRACKET) is None:
                if self.match(TokenType.DOT) is not None:
                    print('ERROR: Encountered dot before closing bracket')
                    exit(1)
                else:
                    jobs.append(self.minor_parse(jobs))
            return OptionalJob(self.current_method, jobs)
        elif cur_type == TokenType.COLON:
            temp = parser_jobs_list[-1]
            parser_jobs_list.pop()
            if type(temp) is MethodJob and temp.method_name == self.current_method:
                temp.save_res_as(None, self.require(TokenType.METHOD).text)
                return temp
            else:
                temp.save_res_as('abc', self.require(TokenType.METHOD).text)
            return temp
        elif cur_type == TokenType.OPAREN:
            variants = []
            while self.parser_pos < len(self.token_list):
                variant = []
                to_exit = False
                while self.match(TokenType.VARIANT) is None:
                    if self.match(TokenType.CPAREN) is not None:
                        to_exit = True
                        break
                    variant.append(self.minor_parse(variant))
                variants.append(variant)
                if to_exit:
                    break
            return VariantJob(self.current_method, variants)
        elif cur_type == TokenType.EXPR_LEFT_ASOC:
            self.require(TokenType.OPAREN)
            job = self.require(TokenType.METHOD).text
            self.require(TokenType.COMMA)
            variants = [self.require(TokenType.TOKEN).text]
            while self.match(TokenType.VARIANT) and not self.parser_pos >= len(self.token_list):
                variants.append(self.require(TokenType.TOKEN).text)
            self.require(TokenType.CPAREN)
            return ExprLeftAsoc(self.current_method, job, variants)
        else:
            print('ERROR: ...2')
            print(cur_type)
            print(parser_jobs_list)
            exit(1)

    def parse(self) -> [Declaration]:
        decls = []
        while self.parser_pos < len(self.token_list):
            name = self.require(TokenType.METHOD)
            self.current_method = name.text

            args = []
            if self.match(TokenType.OPAREN):
                args.append(self.match(TokenType.METHOD).text)
                while self.match(TokenType.COMMA):
                    args.append(self.match(TokenType.METHOD).text)
                self.require(TokenType.CPAREN)

            self_return = False
            if self.match(TokenType.SELF_RETURN):
                self_return = True

            self.require(TokenType.ASSIGN)

            jobs: [Job] = []
            while self.parser_pos < len(self.token_list) and self.match(TokenType.DOT) is None:
                jobs.append(self.minor_parse(jobs))

            decls.append(Declaration(name.text, jobs, args, self_return))

        return check_for_expressions(decls)


def check_for_expressions(decls: [Declaration]) -> [Declaration]:
    for decl in decls:
        if len(decl.jobs) != 1:
            continue

        main_job = decl.jobs[0]

        if not isinstance(main_job, VariantJob):
            continue

        if len(main_job.variants) != 2:
            continue

        if not (len(main_job.variants[0]) == 1 and len(main_job.variants[1]) == 3 or
                len(main_job.variants[1]) == 1 and len(main_job.variants[0]) == 3):
            continue

        primary_job = main_job.variants[0][0] if len(main_job.variants[0]) == 1 else main_job.variants[1][0]
        expr_job = main_job.variants[0] if len(main_job.variants[0]) == 3 else main_job.variants[1]

        if not isinstance(primary_job, MethodJob):
            continue

        if not isinstance(expr_job[0], MethodJob) or not isinstance(expr_job[2], MethodJob):
            continue

        final_job_job = primary_job.method_name
        final_job_operators = []

        if isinstance(expr_job[1], TokenJob):
            final_job_operators.append(expr_job[1].token)
        elif isinstance(expr_job[1], VariantJob):
            to_continue = False
            for e in expr_job[1].variants:
                if len(e) != 1 or not isinstance(e[0], TokenJob):
                    to_continue = True
                    break
                else:
                    final_job_operators.append(e[0].token)
            if to_continue:
                continue
        else:
            continue

        if expr_job[0].method_name == decl.name:
            decl.jobs = [ExprLeftAsoc(decl.name, final_job_job, final_job_operators)]
        elif expr_job[2].method_name == decl.name:
            decl.jobs = [ExprRightAsoc(decl.name, final_job_job, final_job_operators)]
        else:
            continue

    return decls
