import sys
from itertools import product

from src import Style

if len(sys.argv) != 3:
    print('Usage: prg.py file style')
    exit(0)

fin = open(sys.argv[1], 'r')
Style.init(sys.argv[2])
Style.flush()

cur_parent = ''

fout = open('gen/include/ASTForwards.hpp', 'w')
Style.write('#ifndef LOX_AST_FORWARDS_HPP')
Style.write('#define LOX_AST_FORWARDS_HPP')
Style.write()
Style.do_namespaces()
Style.write('namespace AST')
Style.start_block()
Style.write('class Visitor;')
Style.write('class NonConstVisitor;')
for line in fin.readlines():
    if line.startswith('*'):
        continue
    production = line.split(':')
    if len(production) != 2:
        continue
    production[0] = production[0].strip()
    production[1] = production[1].strip()
    if production[1] == '{':
        cur_parent = production[0]
        Style.write()
        Style.write('class ' + cur_parent + ';')
        continue
    Style.write('class ' + production[0] + cur_parent + ';')
Style.end_block(' // namespace AST')
Style.undo_namespace()
Style.write()
Style.write('#endif // LOX_AST_FORWARDS_HPP')
fout.write(Style.flush())
fout.close()
fin.seek(0)
cur_parent = ''

fout = open('gen/include/Visitor.hpp', 'w')
Style.write('#ifndef LOX_VISITOR_HPP')
Style.write('#define LOX_VISITOR_HPP')
Style.write()
Style.write('#include <ASTForwards.hpp>')
Style.write()
Style.do_namespaces()
Style.write('namespace AST')
Style.start_block()
Style.write('class Visitor')
Style.start_block()
Style.publics()
Style.write('virtual ~Visitor() {}')
for line in fin.readlines():
    if line.startswith('*'):
        continue
    production = line.split(':')
    if len(production) != 2:
        continue
    production[0] = production[0].strip()
    production[1] = production[1].strip()
    if production[1] == '{':
        cur_parent = production[0]
        if cur_parent != '':
            Style.write()
        continue
    tolowered = production[0]
    tolowered = tolowered[0].lower() + tolowered[1:]
    Style.write('virtual void visit' + production[0] + cur_parent + '(const AST::' + production[0] + cur_parent + '& ' + cur_parent.lower() + ') = 0;')
Style.end_block('; // class Visitor')
Style.end_block(' // namespace AST')
Style.undo_namespace()
Style.write()
Style.write('#endif // LOX_VISITOR_HPP')
fout.write(Style.flush())
fout.close()
fin.seek(0)
cur_parent = ''

fout = open('gen/include/NonConstVisitor.hpp', 'w')
Style.write('#ifndef LOX_NON_CONST_VISITOR_HPP')
Style.write('#define LOX_NON_CONST_VISITOR_HPP')
Style.write()
Style.write('#include <ASTForwards.hpp>')
Style.write()
Style.do_namespaces()
Style.write('namespace AST')
Style.start_block()
Style.write('class NonConstVisitor')
Style.start_block()
Style.publics()
Style.write('virtual ~NonConstVisitor() {}')
#Style.write()
for line in fin.readlines():
    if line.startswith('*'):
        continue
    production = line.split(':')
    if len(production) != 2:
        continue
    production[0] = production[0].strip()
    production[1] = production[1].strip()
    if production[1] == '{':
        cur_parent = production[0]
        if cur_parent != '':
            Style.write()
        continue
    tolowered = production[0]
    tolowered = tolowered[0].lower() + tolowered[1:]
    Style.write('virtual void visit' + production[0] + cur_parent + '(' + production[0] + cur_parent + '& ' + cur_parent.lower() + ') = 0;')
Style.end_block('; // class NonConstVisitor')
Style.end_block(' // namespace AST')
Style.undo_namespace()
Style.write()
Style.write('#endif // LOX_NON_CONST_VISITOR_HPP')
fout.write(Style.flush())
fout.close()
fin.seek(0)
cur_parent = ''

Style.write('#ifndef LOX_AST_HPP_INCLUDED')
Style.write('#define LOX_AST_HPP_INCLUDED')
Style.write('')
Style.write('#include <memory>')
Style.write()
Style.write('#include <ASTForwards.hpp>')
Style.write('#include <Visitor.hpp>')
Style.write('#include <NonConstVisitor.hpp>')
Style.write('#include <Token.hpp>')
Style.write()
Style.write('namespace Lox')
Style.start_block()
Style.write('namespace AST')
Style.start_block()
Style.write('class Node')
Style.start_block()
Style.tabLevel -= 1
Style.write('public:')
Style.tabLevel += 1
Style.write('virtual ~Node() {}')
Style.write('virtual void accept(Visitor* visitor) const = 0;')
Style.write('virtual void accept(NonConstVisitor* visitor) = 0;')
Style.end_block(';')
Style.write()

for line in fin.readlines():
    if line.startswith('*'):
        continue
    production = line.split(':')
    if len(production) != 2:
        continue
    production[0] = production[0].strip()
    production[1] = production[1].strip()
    if production[1] == '{':
        cur_parent = production[0]
        Style.write('class ' + production[0] + ' : public Node')
        Style.start_block()
        Style.end_block(';')
        Style.write()
    else:
        Style.write('class ' + production[0] + cur_parent + ' : public ' + cur_parent)
        Style.start_block()
        Style.tabLevel -= 1
        Style.write('public:')
        Style.tabLevel += 1
        cons_str = production[0] + cur_parent + '('
        fields = production[1].split(',')
        for field in fields:
            filed_arr = field.strip().split(' ')
            filed_arr[0] = filed_arr[0].strip()
            filed_arr[1] = filed_arr[1].strip()
            if filed_arr[0] != 'Token_t':
                cons_str += 'std::unique_ptr<' + filed_arr[0] + '> '
                cons_str += filed_arr[1] + ', '
            else:
                cons_str += filed_arr[0] + ' '
                cons_str += filed_arr[1] + ', '
        Style.write(cons_str[:-2] + ')')
        Style.tabLevel += 1
        init_str = ': '
        for field in fields:
            filed_arr = field.strip().split(' ')
            filed_arr[0] = filed_arr[0].strip()
            filed_arr[1] = filed_arr[1].strip()
            if filed_arr[0] != 'Token_t':
                init_str += Style.member(filed_arr[1]) + '(std::move(' + filed_arr[1] + ')), '
            else:
                init_str += Style.member(filed_arr[1]) + '(' + filed_arr[1] + '), '
        Style.write(init_str[:-2])
        Style.tabLevel -= 1
        Style.write('{ }')
        Style.write()

        Style.write('void accept(Visitor* visitor) const')
        Style.write('{ visitor->visit' + production[0] + cur_parent + '(*this); }')
        Style.write('')
        Style.write('void accept(NonConstVisitor* visitor)')
        Style.write('{ visitor->visit' + production[0] + cur_parent + '(*this); }')
        Style.write('')

        for field in fields:
            filed_arr = field.strip().split(' ')
            filed_arr[0] = filed_arr[0].strip()
            filed_arr[1] = filed_arr[1].strip()
            Style.write('const ' + filed_arr[0] + '& ' + filed_arr[1] + '() const')
            method_str = '{ '
            if filed_arr[0] != 'Token_t':
                method_str += 'return *' + Style.member(filed_arr[1]) + '; }'
            else:
                method_str += 'return ' + Style.member(filed_arr[1]) + '; }'
            Style.write(method_str)
            Style.write()

        for field in fields:
            filed_arr = field.strip().split(' ')
            filed_arr[0] = filed_arr[0].strip()
            filed_arr[1] = filed_arr[1].strip()
            Style.write('' + filed_arr[0] + '& ' + filed_arr[1] + '()')
            method_str = '{ '
            if filed_arr[0] != 'Token_t':
                method_str += 'return *' + Style.member(filed_arr[1]) + '; }'
            else:
                method_str += 'return ' + Style.member(filed_arr[1]) + '; }'
            Style.write(method_str)
            Style.write()

        Style.tabLevel -= 1
        Style.write('private:')
        Style.tabLevel += 1
        for field in fields:
            filed_arr = field.strip().split(' ')
            filed_arr[0] = filed_arr[0].strip()
            filed_arr[1] = filed_arr[1].strip()
            if filed_arr[0] != 'Token_t':
                Style.write('std::unique_ptr<' + filed_arr[0] + '> ' + Style.member(filed_arr[1]) + ';')
            else:
                Style.write(filed_arr[0] + ' ' + Style.member(filed_arr[1]) + ';')

        Style.end_block(';')
        Style.write()

Style.end_block(' // namespace AST')
Style.write('')
Style.end_block(' // namespace Lox')
Style.write('')
Style.write('#endif // LOX_AST_HPP_INCLUDED')

fout = open('gen/include/AST.hpp', 'w')
fout.write(Style.flush())
fout.close()
fin.seek(0)

visitors = []

for line in fin.readlines():
    if line.startswith('*'):
        visitors.append(line[1:])

fin.seek(0)
Style.flush()

for visitor in visitors:
    production = visitor.split(':')
    production[0] = production[0].strip()
    production[1] = production[1].strip()

    is_mutable = False
    if production[0].endswith('^'):
        is_mutable = True
        production[0] = production[0][:-1]

    cur_ptrs = []

    foutvh = open('gen/include/' + production[0] + 'Visitor.hpp', 'w')

    Style.write('#ifndef LOX_' + production[0].upper() + '_HPP')
    Style.write('#define LOX_' + production[0].upper() + '_HPP')
    Style.write()
    Style.write('#include <' + ('NonConstVisitor' if is_mutable else 'Visitor') + '.hpp>')
    Style.write()
    Style.do_namespaces()

    Style.write('class ' + production[0] + ' : public AST::' + ('NonConstVisitor' if is_mutable else 'Visitor'))
    Style.start_block()
    Style.publics()
    Style.write(production[0] + '();')
    Style.write('~' + production[0] + '();')

    fin.seek(0)
    cur_parent = ''
    for line in fin.readlines():
        if line.startswith('*'):
            continue
        productionp = line.split(':')
        if len(productionp) != 2:
            continue
        productionp[0] = productionp[0].strip()
        productionp[1] = productionp[1].strip()
        if productionp[1] == '{':
            cur_parent = productionp[0]
            cur_ptrs.append(cur_parent)
            if cur_parent != '':
                Style.write()
            continue
        tolowered = productionp[0]
        tolowered = tolowered[0].lower() + tolowered[1:]
        Style.write('void visit' + productionp[0] + cur_parent + '(' + ('' if is_mutable else 'const ') + 'AST::' + productionp[
            0] + cur_parent + '& ' + cur_parent.lower() + ');')

    Style.privates()
    fields = production[1].split(',')
    for i in range(len(fields)):
        Style.write('void ' + fields[i].strip() + '(' + ('' if is_mutable else 'const ') + 'AST::' + cur_ptrs[i] + ' ' + cur_ptrs[i].lower() + ');')
    Style.end_block('; // class ' + production[0])

    Style.undo_namespace()
    Style.write()
    Style.write('#endif // LOX_' + production[0].upper() + '_HPP')

    foutvh.write(Style.flush())
    foutvh.close()

    foutvc = open('gen/src/' + production[0] + 'Visitor.cpp', 'w')
    Style.write('#include <' + production[0] + 'Visitor.hpp>')
    Style.write('#include <AST.hpp>')
    Style.write('#include <ReportsManager.hpp>')
    Style.write()
    Style.do_namespaces()

    Style.write(production[0] + 'Visitor::' + production[0] + 'Visitor()')
    Style.start_block()
    Style.write()
    Style.end_block()

    Style.write('~' + production[0] + 'Visitor::' + production[0] + 'Visitor()')
    Style.start_block()
    Style.write()
    Style.end_block()

    fin.seek(0)
    for line in fin.readlines():
        if line.startswith('*'):
            continue
        productionp = line.split(':')
        if len(productionp) != 2:
            continue
        productionp[0] = productionp[0].strip()
        productionp[1] = productionp[1].strip()
        if productionp[1] == '{':
            cur_parent = productionp[0]
            cur_ptrs.append(cur_parent)
            if cur_parent != '':
                Style.write()
            continue
        tolowered = productionp[0]
        tolowered = tolowered[0].lower() + tolowered[1:]
        Style.write(
            'void ' + production[0] + 'Visitor::visit' + productionp[0] + cur_parent + '(' + ('' if is_mutable else 'const ') + 'AST::' + productionp[
                0] + cur_parent + '& ' + cur_parent.lower() + ')')
        Style.start_block()
        Style.write()
        Style.end_block()
        Style.write()

    Style.undo_namespace()

    foutvc.write(Style.flush())
    foutvc.close()

fin.close()
