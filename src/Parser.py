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

        return decls
