import json
import sys

file_style = {}
tabLevel: int = 0
buf: str = ''
inited: bool = False


def init(file_name: str):
    global inited
    global file_style
    assert (not inited)
    f = None
    try:
        f = open(file_name, 'r')
    except IOError as e:
        print('Can\'t open file: ' + file_name)
    except:
        print('Unknown exception: ' + str(sys.exc_info()[0]))

    with f:
        file_style = json.load(f)

    # TODO: Check style file for correctness

    inited = True


def flush() -> str:
    assert inited
    global buf
    temp = buf
    buf = ''
    return temp


def write(string: str):
    assert inited
    global buf
    buf += (file_style['tab'] * tabLevel) + string + '\n'


def define(what: str) -> str:
    global file_style
    assert inited
    res = ''
    if file_style['globalNamespace'] != '':
        res += file_style['globalNamespace'].upper() + '_'
    if file_style['namespace'] != '':
        res += file_style['namespace'].upper() + '_'
    res += what.upper() + '_HPP_DEFINED'
    return res


def parser_name() -> str:
    assert inited
    return file_style['parserClassName']


def global_namespace() -> str:
    assert inited
    return file_style['globalNamespace']


def namespace() -> str:
    assert inited
    return file_style['namespace']


def token_list() -> str:
    assert inited
    return file_style['tokenListType']


def __pattern(pattern_name: str, what: str) -> str:
    assert inited
    return file_style[pattern_name]['prefix'] + what + file_style[pattern_name]['postfix']


def node(what: str) -> str:
    assert inited
    if file_style['nodeNamePattern']['namespace'] != '':
        return file_style['nodeNamePattern']['namespace'] + '::' + __pattern('nodeNamePattern', what)
    else:
        return __pattern('nodeNamePattern', what)


def node_type(what: str) -> str:
    assert inited
    return file_style['nodeType']['prefix'] + node(what) + file_style['nodeType']['postfix']


def parse(what: str) -> str:
    assert inited
    return file_style['parserMethodPattern']['prefix'] + what + file_style['parserMethodPattern']['postfix']


def member(what: str) -> str:
    assert inited
    return __pattern('memberPattern', what)


def temp(what: str) -> str:
    assert inited
    return __pattern('temporaryVarPattern', what)


def token(what: str = None) -> str:
    assert inited
    if what:
        return file_style['tokenType'] + ' ' + what
    else:
        return file_style['tokenType']


def token_type(what: str = None) -> str:
    assert inited
    if what:
        return file_style['tokenTypeEnumName'] + '::' + what
    else:
        return file_style['tokenTypeEnumName']


def token_ref() -> str:
    assert inited
    return file_style['tokenTypeRef']


def require_decl(definition: bool = False) -> str:
    assert inited
    return token_ref() + ' ' + file_style['parserClassName'] + '::' + file_style['parserRequire'] + '(' + token_type() + ' type)' + (
        ';' if not definition else '')


def match_decl(definition: bool = False) -> str:
    assert inited
    return token_ref() + ' ' + file_style['parserClassName'] + '::' + file_style['parserMatch'] + '(' + token_type() + ' type)' + (
        ';' if not definition else '')


def matching_decl(definition: bool = False) -> str:
    assert inited
    return 'bool ' + file_style['parserClassName'] + '::' + file_style['parserMatching'] + '(' + token_type() + ' type)' + (';' if not definition else '')


def current_token_decl(definition: bool = False) -> str:
    assert inited
    if definition:
        return token_ref() + ' ' + file_style['parserClassName'] + '::' + file_style['parserCurrentToken'] \
               + ('(size_t offset = 0);' if not definition else '(size_t offset)')
    return token_ref() + ' ' + file_style['parserCurrentToken'] \
           + ('(size_t offset = 0);' if not definition else '(size_t offset)')


def pch() -> str:
    assert inited
    return file_style['pch']


def do_namespaces():
    assert inited
    global tabLevel

    if global_namespace():
        write('namespace ' + global_namespace())
        tabLevel += 1

    if namespace():
        write('namespace ' + namespace())
        write('{')
        tabLevel += 1


def undo_namespace():
    assert inited
    global tabLevel

    if namespace():
        tabLevel -= 1
        write('} // ' + namespace())

    if global_namespace():
        tabLevel -= 1
        write('} // ' + global_namespace())


def start_block():
    assert inited
    global tabLevel

    write('{')
    tabLevel += 1


def end_block():
    global tabLevel
    assert tabLevel != 0

    tabLevel -= 1
    write('}')


def match(tok_type: str, literally: bool = False) -> str:
    assert inited
    return file_style['parserMatch'] + '(' + (token_type(tok_type) if not literally else tok_type) + ')'


def current_token(offset: int = 0) -> str:
    assert inited
    return file_style['parserCurrentToken'] + '(' + (str(offset) if offset != 0 else '') + ')'


def parser_pos() -> str:
    assert inited
    return file_style['parserPosition']


def null_token() -> str:
    assert inited
    return file_style['nullToken']


def parser_tokens() -> str:
    assert inited
    return file_style['parserTokens']


def parser_hpp() -> str:
    assert inited
    return file_style['parserHeader']


def lexer_hpp() -> str:
    assert inited
    return file_style['lexerHeader']


def ast_hpp() -> str:
    assert inited
    return file_style['ASTHeader']


def require(what: str) -> str:
    assert inited
    return file_style['parserRequire'] + '(' + token_type(what) + ')'


def parser_cpp() -> str:
    assert inited
    return file_style['parserSource']


def container_type(what: str) -> str:
    assert inited
    return __pattern('containerPattern', what)


def current_token_equals_type(what: str, pos: int = 0) -> str:
    assert inited
    return current_token(pos) + '.type == ' + token_type(what)


def matching(what: str) -> str:
    assert inited
    return file_style['parserMatching'] + '(' + token_type(what) + ')'


def previous_token(offset: int = 0) -> str:
    assert inited
    return file_style['parserPreviousToken'] + '(' + (str(offset) if offset != 0 else '') + ')'


def abstract_node_type() -> str:
    assert inited
    return node_type(file_style['abstractNode'])


def node_namespace() -> str:
    assert inited
    return