import sys

from src import Style, Tokens
from src.Parser import Parser

if len(sys.argv) == 2 or len(sys.argv) > 3:
    print('ERROR: Wrong arguments count.\nUsage: script.py fileName styleFile')
else:
    Style.init(sys.argv[2])

    with open(sys.argv[1], 'r') as fin:
        source = fin.read()
    tokenList = Tokens.gen_token_list(source)
    p = Parser(tokenList)

    decls = p.parse()

    # 1. Generate Parser.hpp
    # 2. Generate Parser.cpp
    # 3. Generate Visitor.hpp
    # 4. Generate AST.hpp
    # 5. Generate Lexer.hpp

    # 1. Parser.hpp
    Style.flush()

    Style.write('#ifndef ' + Style.define(Style.parser_name()))
    Style.write('#define ' + Style.define(Style.parser_name()))
    Style.write('')
    Style.write('#include <memory>')
    Style.write('')
    Style.write('#include <' + Style.lexer_hpp() + '>')
    Style.write('#include <' + Style.ast_hpp() + '>')
    Style.write('')

    Style.do_namespaces()

    Style.write('class ' + Style.parser_name())
    Style.write('{')
    Style.write('public:')
    Style.tabLevel += 1

    Style.write(Style.parser_name() + '(' + Style.token_list() + ' tokens);')
    Style.write('')

    for decl in decls:
        Style.write(Style.node_type(decl.name) + ' ' + Style.parse(decl.name) + '();')
    Style.write('')
    Style.tabLevel -= 1
    Style.write('private:')
    Style.tabLevel += 1
    Style.write(Style.token_list() + ' ' + Style.member(Style.parser_tokens()) + ';')
    Style.write('size_t ' + Style.member(Style.parser_pos()) + ';')
    Style.write(Style.token() + ' ' + Style.null_token() + ' = { ' + Style.token_type('NONE') + ', "", 0 };')
    Style.write('')

    Style.write(Style.require_decl())
    Style.write(Style.match_decl())
    Style.write(Style.matching_decl())
    Style.write('')
    Style.write(Style.current_token_decl())

    Style.tabLevel -= 1
    Style.write('} // ' + Style.parser_name())

    Style.undo_namespace()

    Style.write('')
    Style.write('#endif // ' + Style.define(Style.parser_name()))
    Style.write('')

    with open('gen/include/' + Style.parser_hpp(), 'w') as f:
        f.write(Style.flush())

    # 2. Generate Parser.cpp
    if Style.pch() != '':
        Style.write('#include <' + Style.pch() + '>')
    Style.write('#include <' + Style.parser_hpp() + '>')
    Style.write('')
    Style.do_namespaces()

    Style.write(Style.parser_name() + '::' + Style.parser_name() + '(' + Style.token_list() + ' tokens)')
    Style.tabLevel += 1
    Style.write(': ' + Style.member(Style.parser_tokens()) + '(tokens)')
    Style.tabLevel -= 1
    Style.write('{ }')
    Style.write('')

    for decl in decls:
        Style.write(Style.node_type(decl.name) + ' ' +
                    Style.parser_name() + '::' + Style.parse(decl.name) + '()')
        Style.start_block()
        decl.generate()
        Style.end_block()
        Style.write('')

    # match
    Style.write(Style.match_decl(True))
    Style.start_block()

    Style.write(Style.token_ref() + ' curTok = ' + Style.current_token() + ';')
    Style.write('if (curTok.type == type)')
    Style.start_block()
    Style.write(Style.member(Style.parser_pos()) + '++;')
    Style.write('return curTok;')
    Style.end_block()
    Style.write('else return ' + Style.null_token() + ';')

    Style.end_block()
    Style.write('')

    # matching
    Style.write(Style.match_decl(True))
    Style.start_block()

    Style.write('return ' + Style.match('type', True) + '.type != ' + Style.token_type('NONE') + ';')

    Style.end_block()
    Style.write('')

    # require
    Style.write(Style.require_decl(True))
    Style.start_block()

    Style.write(Style.token_ref() + ' curTok = ' + Style.match('type') + ';')
    Style.write('if (curTok.type != ' + Style.token_type('NONE') + ')')
    Style.start_block()
    Style.write('return curTok;')
    Style.end_block()
    Style.write('// TODO: else error();')

    Style.end_block()
    Style.write('')

    # currentToken
    Style.write(Style.current_token_decl(True))
    Style.start_block()

    Style.write('if (' + Style.member(Style.parser_pos()) + ' < ' + Style.member(Style.parser_tokens()) + '->size())')
    Style.start_block()
    Style.write('return (*' + Style.member(Style.parser_tokens()) + ')[' + Style.member(Style.parser_pos()) + '];')
    Style.end_block()
    Style.write('else return ' + Style.null_token() + ';')

    Style.end_block()
    Style.write('')

    Style.undo_namespace()
    Style.write('')

    with open('gen/src/' + Style.parser_cpp(), 'w') as f:
        f.write(Style.flush())