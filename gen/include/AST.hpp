#ifndef LOX_AST_HPP_INCLUDED
#define LOX_AST_HPP_INCLUDED

namespace Lox
{
    namespace AST
    {
        class Visitor;
        class Node
        {
        public:
            virtual ~Node() {}
            virtual void accept(Visitor visitor) = 0;
        };
        
        class Stmt : public Node
        {
        };
        
        class Expression : public Stmt
        {
        public:
            Expression(std::unique_ptr<Expr> expr)
                : m_expr(std::move(expr))
            { }
        };
        
        class VarDecl : public Stmt
        {
        public:
            VarDecl(Token_t name, std::unique_ptr<Expr> initializer)
                : m_name(name), m_initializer(std::move(initializer))
            { }
        };
        
        class If : public Stmt
        {
        public:
            If(std::unique_ptr<Expr> condition, std::unique_ptr<Stmt> trueStmt, std::unique_ptr<Stmt> falseStmt)
                : m_condition(std::move(condition)), m_trueStmt(std::move(trueStmt)), m_falseStmt(std::move(falseStmt))
            { }
        };
        
        class While : public Stmt
        {
        public:
            While(std::unique_ptr<Expr> condition, std::unique_ptr<Stmt> body)
                : m_condition(std::move(condition)), m_body(std::move(body))
            { }
        };
        
        class DoWhile : public Stmt
        {
        public:
            DoWhile(std::unique_ptr<Expr> condition, std::unique_ptr<Stmt> body)
                : m_condition(std::move(condition)), m_body(std::move(body))
            { }
        };
        
        class For : public Stmt
        {
        public:
            For(std::unique_ptr<Stmt> initializer, std::unique_ptr<Expr> condition, std::unique_ptr<Expr> increment, std::unique_ptr<Stmt> body)
                : m_initializer(std::move(initializer)), m_condition(std::move(condition)), m_increment(std::move(increment)), m_body(std::move(body))
            { }
        };
        
        class Break : public Stmt
        {
        public:
            Break(Token_t keyword)
                : m_keyword(keyword)
            { }
        };
        
        class Continue : public Stmt
        {
        public:
            Continue(Token_t keyword)
                : m_keyword(keyword)
            { }
        };
        
        class Print : public Stmt
        {
        public:
            Print(std::unique_ptr<Expr> value)
                : m_value(std::move(value))
            { }
        };
        
        class Return : public Stmt
        {
        public:
            Return(Token_t keyword, std::unique_ptr<Expr> value)
                : m_keyword(keyword), m_value(std::move(value))
            { }
        };
        
        class Expr : public Node
        {
        };
        
        class Literal : public Expr
        {
        public:
            Literal(Token_t value)
                : m_value(value)
            { }
        };
        
        class Var : public Expr
        {
        public:
            Var(Token_t name)
                : m_name(name)
            { }
        };
        
        class Group : public Expr
        {
        public:
            Group(std::unique_ptr<Expr> expr)
                : m_expr(std::move(expr))
            { }
        };
        
        class Unary : public Expr
        {
        public:
            Unary(Token_t operation, std::unique_ptr<Expr> expr)
                : m_operation(operation), m_expr(std::move(expr))
            { }
        };
        
        class PostIncrement : public Expr
        {
        public:
            PostIncrement(Token_t operation, std::unique_ptr<VarExpr> var)
                : m_operation(operation), m_var(std::move(var))
            { }
        };
        
        class PreIncrement : public Expr
        {
        public:
            PreIncrement(Token_t operation, std::unique_ptr<VarExpr> var)
                : m_operation(operation), m_var(std::move(var))
            { }
        };
        
        class Binary : public Expr
        {
        public:
            Binary(std::unique_ptr<Expr> left, Token_t operation, std::unique_ptr<Expr> right)
                : m_left(std::move(left)), m_operation(operation), m_right(std::move(right))
            { }
        };
        
        class Ternary : public Expr
        {
        public:
            Ternary(std::unique_ptr<Expr> mainExpr, std::unique_ptr<Expr> trueExpr, std::unique_ptr<Expr> falseExpr)
                : m_mainExpr(std::move(mainExpr)), m_trueExpr(std::move(trueExpr)), m_falseExpr(std::move(falseExpr))
            { }
        };
        
        class Assign : public Expr
        {
        public:
            Assign(std::unique_ptr<Expr.Var> lvalue, Token_t operation, std::unique_ptr<Expr> rvalue)
                : m_lvalue(std::move(lvalue)), m_operation(operation), m_rvalue(std::move(rvalue))
            { }
        };
        
        class Logical : public Expr
        {
        public:
            Logical(std::unique_ptr<Expr> left, Token_t operation, std::unique_ptr<Expr> right)
                : m_left(std::move(left)), m_operation(operation), m_right(std::move(right))
            { }
        };
        
        class Get : public Expr
        {
        public:
            Get(std::unique_ptr<Expr> object, std::unique_ptr<VarExpr> name)
                : m_object(std::move(object)), m_name(std::move(name))
            { }
        };
        
        class Set : public Expr
        {
        public:
            Set(std::unique_ptr<Expr> object, std::unique_ptr<VarExpr> name, Token_t operation, std::unique_ptr<Expr> value)
                : m_object(std::move(object)), m_name(std::move(name)), m_operation(operation), m_value(std::move(value))
            { }
        };
        
        class Index : public Expr
        {
        public:
            Index(std::unique_ptr<Expr> object, std::unique_ptr<Expr> index, Token_t openBracket)
                : m_object(std::move(object)), m_index(std::move(index)), m_openBracket(openBracket)
            { }
        };
        
        class Super : public Expr
        {
        public:
            Super(Token_t keyword, Token_t method)
                : m_keyword(keyword), m_method(method)
            { }
        };
        
    } // namespace AST
    
} // namespace Lox

#endif // LOX_AST_HPP_INCLUDED
