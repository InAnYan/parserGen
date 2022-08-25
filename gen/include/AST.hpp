#ifndef LOX_AST_HPP_INCLUDED
#define LOX_AST_HPP_INCLUDED

#include <memory>

#include <ASTForwards.hpp>
#include <Visitor.hpp>
#include <NonConstVisitor.hpp>
#include <Token.hpp>

namespace Lox
{
    namespace AST
    {
        class Node
        {
        public:
            virtual ~Node() {}
            virtual void accept(Visitor* visitor) const = 0;
            virtual void accept(NonConstVisitor* visitor) = 0;
        };
        
        class Stmt : public Node
        {
        };
        
        class ExpressionStmt : public Stmt
        {
        public:
            ExpressionStmt(std::unique_ptr<Expr> expr)
                : m_expr(std::move(expr))
            { }
            
            void accept(Visitor* visitor) const
            { visitor->visitExpressionStmt(*this); }
            
            void accept(NonConstVisitor* visitor)
            { visitor->visitExpressionStmt(*this); }
            
            const Expr& expr() const
            { return *m_expr; }
            
            Expr& expr()
            { return *m_expr; }
            
        private:
            std::unique_ptr<Expr> m_expr;
        };
        
        class VarDeclStmt : public Stmt
        {
        public:
            VarDeclStmt(Token_t name, std::unique_ptr<Expr> initializer)
                : m_name(name), m_initializer(std::move(initializer))
            { }
            
            void accept(Visitor* visitor) const
            { visitor->visitVarDeclStmt(*this); }
            
            void accept(NonConstVisitor* visitor)
            { visitor->visitVarDeclStmt(*this); }
            
            const Token_t& name() const
            { return m_name; }
            
            const Expr& initializer() const
            { return *m_initializer; }
            
            Token_t& name()
            { return m_name; }
            
            Expr& initializer()
            { return *m_initializer; }
            
        private:
            Token_t m_name;
            std::unique_ptr<Expr> m_initializer;
        };
        
        class IfStmt : public Stmt
        {
        public:
            IfStmt(std::unique_ptr<Expr> condition, std::unique_ptr<Stmt> trueStmt, std::unique_ptr<Stmt> falseStmt)
                : m_condition(std::move(condition)), m_trueStmt(std::move(trueStmt)), m_falseStmt(std::move(falseStmt))
            { }
            
            void accept(Visitor* visitor) const
            { visitor->visitIfStmt(*this); }
            
            void accept(NonConstVisitor* visitor)
            { visitor->visitIfStmt(*this); }
            
            const Expr& condition() const
            { return *m_condition; }
            
            const Stmt& trueStmt() const
            { return *m_trueStmt; }
            
            const Stmt& falseStmt() const
            { return *m_falseStmt; }
            
            Expr& condition()
            { return *m_condition; }
            
            Stmt& trueStmt()
            { return *m_trueStmt; }
            
            Stmt& falseStmt()
            { return *m_falseStmt; }
            
        private:
            std::unique_ptr<Expr> m_condition;
            std::unique_ptr<Stmt> m_trueStmt;
            std::unique_ptr<Stmt> m_falseStmt;
        };
        
        class WhileStmt : public Stmt
        {
        public:
            WhileStmt(std::unique_ptr<Expr> condition, std::unique_ptr<Stmt> body)
                : m_condition(std::move(condition)), m_body(std::move(body))
            { }
            
            void accept(Visitor* visitor) const
            { visitor->visitWhileStmt(*this); }
            
            void accept(NonConstVisitor* visitor)
            { visitor->visitWhileStmt(*this); }
            
            const Expr& condition() const
            { return *m_condition; }
            
            const Stmt& body() const
            { return *m_body; }
            
            Expr& condition()
            { return *m_condition; }
            
            Stmt& body()
            { return *m_body; }
            
        private:
            std::unique_ptr<Expr> m_condition;
            std::unique_ptr<Stmt> m_body;
        };
        
        class DoWhileStmt : public Stmt
        {
        public:
            DoWhileStmt(std::unique_ptr<Expr> condition, std::unique_ptr<Stmt> body)
                : m_condition(std::move(condition)), m_body(std::move(body))
            { }
            
            void accept(Visitor* visitor) const
            { visitor->visitDoWhileStmt(*this); }
            
            void accept(NonConstVisitor* visitor)
            { visitor->visitDoWhileStmt(*this); }
            
            const Expr& condition() const
            { return *m_condition; }
            
            const Stmt& body() const
            { return *m_body; }
            
            Expr& condition()
            { return *m_condition; }
            
            Stmt& body()
            { return *m_body; }
            
        private:
            std::unique_ptr<Expr> m_condition;
            std::unique_ptr<Stmt> m_body;
        };
        
        class ForStmt : public Stmt
        {
        public:
            ForStmt(std::unique_ptr<Stmt> initializer, std::unique_ptr<Expr> condition, std::unique_ptr<Expr> increment, std::unique_ptr<Stmt> body)
                : m_initializer(std::move(initializer)), m_condition(std::move(condition)), m_increment(std::move(increment)), m_body(std::move(body))
            { }
            
            void accept(Visitor* visitor) const
            { visitor->visitForStmt(*this); }
            
            void accept(NonConstVisitor* visitor)
            { visitor->visitForStmt(*this); }
            
            const Stmt& initializer() const
            { return *m_initializer; }
            
            const Expr& condition() const
            { return *m_condition; }
            
            const Expr& increment() const
            { return *m_increment; }
            
            const Stmt& body() const
            { return *m_body; }
            
            Stmt& initializer()
            { return *m_initializer; }
            
            Expr& condition()
            { return *m_condition; }
            
            Expr& increment()
            { return *m_increment; }
            
            Stmt& body()
            { return *m_body; }
            
        private:
            std::unique_ptr<Stmt> m_initializer;
            std::unique_ptr<Expr> m_condition;
            std::unique_ptr<Expr> m_increment;
            std::unique_ptr<Stmt> m_body;
        };
        
        class BreakStmt : public Stmt
        {
        public:
            BreakStmt(Token_t keyword)
                : m_keyword(keyword)
            { }
            
            void accept(Visitor* visitor) const
            { visitor->visitBreakStmt(*this); }
            
            void accept(NonConstVisitor* visitor)
            { visitor->visitBreakStmt(*this); }
            
            const Token_t& keyword() const
            { return m_keyword; }
            
            Token_t& keyword()
            { return m_keyword; }
            
        private:
            Token_t m_keyword;
        };
        
        class ContinueStmt : public Stmt
        {
        public:
            ContinueStmt(Token_t keyword)
                : m_keyword(keyword)
            { }
            
            void accept(Visitor* visitor) const
            { visitor->visitContinueStmt(*this); }
            
            void accept(NonConstVisitor* visitor)
            { visitor->visitContinueStmt(*this); }
            
            const Token_t& keyword() const
            { return m_keyword; }
            
            Token_t& keyword()
            { return m_keyword; }
            
        private:
            Token_t m_keyword;
        };
        
        class PrintStmt : public Stmt
        {
        public:
            PrintStmt(std::unique_ptr<Expr> value)
                : m_value(std::move(value))
            { }
            
            void accept(Visitor* visitor) const
            { visitor->visitPrintStmt(*this); }
            
            void accept(NonConstVisitor* visitor)
            { visitor->visitPrintStmt(*this); }
            
            const Expr& value() const
            { return *m_value; }
            
            Expr& value()
            { return *m_value; }
            
        private:
            std::unique_ptr<Expr> m_value;
        };
        
        class ReturnStmt : public Stmt
        {
        public:
            ReturnStmt(Token_t keyword, std::unique_ptr<Expr> value)
                : m_keyword(keyword), m_value(std::move(value))
            { }
            
            void accept(Visitor* visitor) const
            { visitor->visitReturnStmt(*this); }
            
            void accept(NonConstVisitor* visitor)
            { visitor->visitReturnStmt(*this); }
            
            const Token_t& keyword() const
            { return m_keyword; }
            
            const Expr& value() const
            { return *m_value; }
            
            Token_t& keyword()
            { return m_keyword; }
            
            Expr& value()
            { return *m_value; }
            
        private:
            Token_t m_keyword;
            std::unique_ptr<Expr> m_value;
        };
        
        class Expr : public Node
        {
        };
        
        class LiteralExpr : public Expr
        {
        public:
            LiteralExpr(Token_t value)
                : m_value(value)
            { }
            
            void accept(Visitor* visitor) const
            { visitor->visitLiteralExpr(*this); }
            
            void accept(NonConstVisitor* visitor)
            { visitor->visitLiteralExpr(*this); }
            
            const Token_t& value() const
            { return m_value; }
            
            Token_t& value()
            { return m_value; }
            
        private:
            Token_t m_value;
        };
        
        class VarExpr : public Expr
        {
        public:
            VarExpr(Token_t name)
                : m_name(name)
            { }
            
            void accept(Visitor* visitor) const
            { visitor->visitVarExpr(*this); }
            
            void accept(NonConstVisitor* visitor)
            { visitor->visitVarExpr(*this); }
            
            const Token_t& name() const
            { return m_name; }
            
            Token_t& name()
            { return m_name; }
            
        private:
            Token_t m_name;
        };
        
        class GroupExpr : public Expr
        {
        public:
            GroupExpr(std::unique_ptr<Expr> expr)
                : m_expr(std::move(expr))
            { }
            
            void accept(Visitor* visitor) const
            { visitor->visitGroupExpr(*this); }
            
            void accept(NonConstVisitor* visitor)
            { visitor->visitGroupExpr(*this); }
            
            const Expr& expr() const
            { return *m_expr; }
            
            Expr& expr()
            { return *m_expr; }
            
        private:
            std::unique_ptr<Expr> m_expr;
        };
        
        class UnaryExpr : public Expr
        {
        public:
            UnaryExpr(Token_t operation, std::unique_ptr<Expr> expr)
                : m_operation(operation), m_expr(std::move(expr))
            { }
            
            void accept(Visitor* visitor) const
            { visitor->visitUnaryExpr(*this); }
            
            void accept(NonConstVisitor* visitor)
            { visitor->visitUnaryExpr(*this); }
            
            const Token_t& operation() const
            { return m_operation; }
            
            const Expr& expr() const
            { return *m_expr; }
            
            Token_t& operation()
            { return m_operation; }
            
            Expr& expr()
            { return *m_expr; }
            
        private:
            Token_t m_operation;
            std::unique_ptr<Expr> m_expr;
        };
        
        class PostIncrementExpr : public Expr
        {
        public:
            PostIncrementExpr(Token_t operation, std::unique_ptr<VarExpr> var)
                : m_operation(operation), m_var(std::move(var))
            { }
            
            void accept(Visitor* visitor) const
            { visitor->visitPostIncrementExpr(*this); }
            
            void accept(NonConstVisitor* visitor)
            { visitor->visitPostIncrementExpr(*this); }
            
            const Token_t& operation() const
            { return m_operation; }
            
            const VarExpr& var() const
            { return *m_var; }
            
            Token_t& operation()
            { return m_operation; }
            
            VarExpr& var()
            { return *m_var; }
            
        private:
            Token_t m_operation;
            std::unique_ptr<VarExpr> m_var;
        };
        
        class PreIncrementExpr : public Expr
        {
        public:
            PreIncrementExpr(Token_t operation, std::unique_ptr<VarExpr> var)
                : m_operation(operation), m_var(std::move(var))
            { }
            
            void accept(Visitor* visitor) const
            { visitor->visitPreIncrementExpr(*this); }
            
            void accept(NonConstVisitor* visitor)
            { visitor->visitPreIncrementExpr(*this); }
            
            const Token_t& operation() const
            { return m_operation; }
            
            const VarExpr& var() const
            { return *m_var; }
            
            Token_t& operation()
            { return m_operation; }
            
            VarExpr& var()
            { return *m_var; }
            
        private:
            Token_t m_operation;
            std::unique_ptr<VarExpr> m_var;
        };
        
        class BinaryExpr : public Expr
        {
        public:
            BinaryExpr(std::unique_ptr<Expr> left, Token_t operation, std::unique_ptr<Expr> right)
                : m_left(std::move(left)), m_operation(operation), m_right(std::move(right))
            { }
            
            void accept(Visitor* visitor) const
            { visitor->visitBinaryExpr(*this); }
            
            void accept(NonConstVisitor* visitor)
            { visitor->visitBinaryExpr(*this); }
            
            const Expr& left() const
            { return *m_left; }
            
            const Token_t& operation() const
            { return m_operation; }
            
            const Expr& right() const
            { return *m_right; }
            
            Expr& left()
            { return *m_left; }
            
            Token_t& operation()
            { return m_operation; }
            
            Expr& right()
            { return *m_right; }
            
        private:
            std::unique_ptr<Expr> m_left;
            Token_t m_operation;
            std::unique_ptr<Expr> m_right;
        };
        
        class TernaryExpr : public Expr
        {
        public:
            TernaryExpr(std::unique_ptr<Expr> mainExpr, std::unique_ptr<Expr> trueExpr, std::unique_ptr<Expr> falseExpr)
                : m_mainExpr(std::move(mainExpr)), m_trueExpr(std::move(trueExpr)), m_falseExpr(std::move(falseExpr))
            { }
            
            void accept(Visitor* visitor) const
            { visitor->visitTernaryExpr(*this); }
            
            void accept(NonConstVisitor* visitor)
            { visitor->visitTernaryExpr(*this); }
            
            const Expr& mainExpr() const
            { return *m_mainExpr; }
            
            const Expr& trueExpr() const
            { return *m_trueExpr; }
            
            const Expr& falseExpr() const
            { return *m_falseExpr; }
            
            Expr& mainExpr()
            { return *m_mainExpr; }
            
            Expr& trueExpr()
            { return *m_trueExpr; }
            
            Expr& falseExpr()
            { return *m_falseExpr; }
            
        private:
            std::unique_ptr<Expr> m_mainExpr;
            std::unique_ptr<Expr> m_trueExpr;
            std::unique_ptr<Expr> m_falseExpr;
        };
        
        class AssignExpr : public Expr
        {
        public:
            AssignExpr(std::unique_ptr<VarExpr> lvalue, Token_t operation, std::unique_ptr<Expr> rvalue)
                : m_lvalue(std::move(lvalue)), m_operation(operation), m_rvalue(std::move(rvalue))
            { }
            
            void accept(Visitor* visitor) const
            { visitor->visitAssignExpr(*this); }
            
            void accept(NonConstVisitor* visitor)
            { visitor->visitAssignExpr(*this); }
            
            const VarExpr& lvalue() const
            { return *m_lvalue; }
            
            const Token_t& operation() const
            { return m_operation; }
            
            const Expr& rvalue() const
            { return *m_rvalue; }
            
            VarExpr& lvalue()
            { return *m_lvalue; }
            
            Token_t& operation()
            { return m_operation; }
            
            Expr& rvalue()
            { return *m_rvalue; }
            
        private:
            std::unique_ptr<VarExpr> m_lvalue;
            Token_t m_operation;
            std::unique_ptr<Expr> m_rvalue;
        };
        
        class LogicalExpr : public Expr
        {
        public:
            LogicalExpr(std::unique_ptr<Expr> left, Token_t operation, std::unique_ptr<Expr> right)
                : m_left(std::move(left)), m_operation(operation), m_right(std::move(right))
            { }
            
            void accept(Visitor* visitor) const
            { visitor->visitLogicalExpr(*this); }
            
            void accept(NonConstVisitor* visitor)
            { visitor->visitLogicalExpr(*this); }
            
            const Expr& left() const
            { return *m_left; }
            
            const Token_t& operation() const
            { return m_operation; }
            
            const Expr& right() const
            { return *m_right; }
            
            Expr& left()
            { return *m_left; }
            
            Token_t& operation()
            { return m_operation; }
            
            Expr& right()
            { return *m_right; }
            
        private:
            std::unique_ptr<Expr> m_left;
            Token_t m_operation;
            std::unique_ptr<Expr> m_right;
        };
        
        class GetExpr : public Expr
        {
        public:
            GetExpr(std::unique_ptr<Expr> object, std::unique_ptr<VarExpr> name)
                : m_object(std::move(object)), m_name(std::move(name))
            { }
            
            void accept(Visitor* visitor) const
            { visitor->visitGetExpr(*this); }
            
            void accept(NonConstVisitor* visitor)
            { visitor->visitGetExpr(*this); }
            
            const Expr& object() const
            { return *m_object; }
            
            const VarExpr& name() const
            { return *m_name; }
            
            Expr& object()
            { return *m_object; }
            
            VarExpr& name()
            { return *m_name; }
            
        private:
            std::unique_ptr<Expr> m_object;
            std::unique_ptr<VarExpr> m_name;
        };
        
        class SetExpr : public Expr
        {
        public:
            SetExpr(std::unique_ptr<Expr> object, std::unique_ptr<VarExpr> name, Token_t operation, std::unique_ptr<Expr> value)
                : m_object(std::move(object)), m_name(std::move(name)), m_operation(operation), m_value(std::move(value))
            { }
            
            void accept(Visitor* visitor) const
            { visitor->visitSetExpr(*this); }
            
            void accept(NonConstVisitor* visitor)
            { visitor->visitSetExpr(*this); }
            
            const Expr& object() const
            { return *m_object; }
            
            const VarExpr& name() const
            { return *m_name; }
            
            const Token_t& operation() const
            { return m_operation; }
            
            const Expr& value() const
            { return *m_value; }
            
            Expr& object()
            { return *m_object; }
            
            VarExpr& name()
            { return *m_name; }
            
            Token_t& operation()
            { return m_operation; }
            
            Expr& value()
            { return *m_value; }
            
        private:
            std::unique_ptr<Expr> m_object;
            std::unique_ptr<VarExpr> m_name;
            Token_t m_operation;
            std::unique_ptr<Expr> m_value;
        };
        
        class IndexExpr : public Expr
        {
        public:
            IndexExpr(std::unique_ptr<Expr> object, std::unique_ptr<Expr> index, Token_t openBracket)
                : m_object(std::move(object)), m_index(std::move(index)), m_openBracket(openBracket)
            { }
            
            void accept(Visitor* visitor) const
            { visitor->visitIndexExpr(*this); }
            
            void accept(NonConstVisitor* visitor)
            { visitor->visitIndexExpr(*this); }
            
            const Expr& object() const
            { return *m_object; }
            
            const Expr& index() const
            { return *m_index; }
            
            const Token_t& openBracket() const
            { return m_openBracket; }
            
            Expr& object()
            { return *m_object; }
            
            Expr& index()
            { return *m_index; }
            
            Token_t& openBracket()
            { return m_openBracket; }
            
        private:
            std::unique_ptr<Expr> m_object;
            std::unique_ptr<Expr> m_index;
            Token_t m_openBracket;
        };
        
        class SuperExpr : public Expr
        {
        public:
            SuperExpr(Token_t keyword, Token_t method)
                : m_keyword(keyword), m_method(method)
            { }
            
            void accept(Visitor* visitor) const
            { visitor->visitSuperExpr(*this); }
            
            void accept(NonConstVisitor* visitor)
            { visitor->visitSuperExpr(*this); }
            
            const Token_t& keyword() const
            { return m_keyword; }
            
            const Token_t& method() const
            { return m_method; }
            
            Token_t& keyword()
            { return m_keyword; }
            
            Token_t& method()
            { return m_method; }
            
        private:
            Token_t m_keyword;
            Token_t m_method;
        };
        
    } // namespace AST
    
} // namespace Lox

#endif // LOX_AST_HPP_INCLUDED
