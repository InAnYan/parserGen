from enum import Enum
import re


class TokenType(Enum):
    DOT = 'DOT'
    ASSIGN = 'ASSIGN'
    VARIANT = 'VARIANT'
    OPAREN = 'OPAREN'
    CPAREN = 'CPAREN'
    OBRACKET = 'OBRACKET'
    CBRACKET = 'CBRACKET'
    OFIG = 'OFIG'
    CFIG = 'CFIG'
    PLUS = 'PLUS'
    NULL = 'NULL'
    TOKEN = 'TOKEN'
    METHOD = 'METHOD'
    COMMA = 'COMMA'
    COLON = 'COLON'
    ASTERISK = 'ASTERISK'
    SELF_RETURN = 'SELF_RETURN'
    EXPR_LEFT_ASOC = 'EXPR_LEFT_ASOC'
    
    
tokenTypesList = [
    (TokenType.EXPR_LEFT_ASOC, 'EXPR_LEFT_ASOC'),
    (TokenType.SELF_RETURN, '\\^'),
    (TokenType.DOT, '\\.'),
    (TokenType.ASSIGN, '::='),
    (TokenType.COLON, ':'),
    (TokenType.VARIANT, '\\|'),
    (TokenType.OPAREN, '\\('),
    (TokenType.CPAREN, '\\)'),
    (TokenType.OBRACKET, '\\['),
    (TokenType.CBRACKET, '\\]'),
    (TokenType.PLUS, '\\+'),
    (TokenType.NULL, '_NULL_'),
    (TokenType.TOKEN, '\"[^\"]+\"'),
    (TokenType.METHOD, '[\\w_]+\\b'),
    (TokenType.COMMA, ','),
    (TokenType.OFIG, '\\{'),
    (TokenType.CFIG, '\\}'),
    (TokenType.ASTERISK, '\\*'),
]


sym_map = {
    '+': 'PLUS',
    '-': 'MINUS',
    '/': 'DIVISION',
    '*': 'ASTERISK',
    ':': 'COLON',
    ';': 'SEMICOLON',
    '%': 'PERCENT',
    '(': 'OPEN_PAREN',
    ')': 'CLOSE_PAREN',
    ',': 'COMMA',
    '=': 'EQUALS',
    '_': 'UNDERSCORE',
    '.': 'DOT',
    ' ': 'NULL',
    '[': 'OPEN_BRACKET',
    ']': 'CLOSE_BRACKET',
    '{': 'OPEN_FIGURE',
    '}': 'CLOSE_FIGURE',
    '!': 'BANG',
    '?': 'QUESTION',
    '>': 'GREATER',
    '<': 'LESS'
}


def chars_to_token(what: str) -> str:
    if what[0].isalpha():
        return what.upper()

    res = ''
    i = 0

    while i < len(what):
        if what[i] == '\\':
            i += 1
            if i < len(what):
                if what[i] == 'w':
                    return 'IDENTIFIER'
                elif what[i] == 'i':
                    return 'INT_LITERAL'
                elif what[i] == 'f':
                    return 'FLOAT_LITERAL'
                else:
                    print('ERROR: Unknown special character: \'' + what[i] + '\'')
                    exit(1)
            else:
                break
        else:
            if what[i] not in sym_map:
                print('ERROR: no such symbol as: \'' + what[i] + '\'. Check your token: \'' + what + '\'')
                exit(1)
            res += sym_map[what[i]]
            if i != len(what) - 1:
                res += '_'

        i += 1

    return res


class Token:
    token_type: TokenType
    text: str
    pos: int

    def __init__(self, token_type, text, pos):
        self.token_type = token_type
        self.text = chars_to_token(text) if token_type == TokenType.TOKEN else text
        self.original = text
        self.pos = pos

    def __str__(self):
        return str(self.token_type) + ': \"' + self.text + '\" (' + str(self.pos) + ')'

    def __repr__(self):
        return self.__str__()


def gen_token_list(source: str) -> [Token]:
    res: [Token] = []
    lexer_pos: int = 0

    while lexer_pos < len(source):
        found = False
        for (tok_type, regex) in tokenTypesList:
            attempt = re.match(regex, source[lexer_pos:])
            if attempt:
                res.append(Token(tok_type, attempt.group(0) if tok_type != TokenType.TOKEN else attempt.group(0)[1:-1],
                                 lexer_pos))
                lexer_pos += len(attempt.group(0))
                while (lexer_pos < len(source)) and source[lexer_pos].isspace():
                    lexer_pos += 1
                found = True
                break
        if not found:
            print('ERROR: Unrecognized tokens at pos ' + str(lexer_pos))
            print(source[lexer_pos:])
            exit(1)

    return res
