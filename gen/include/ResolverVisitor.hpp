#ifndef LOX_RESOLVER_HPP
#define LOX_RESOLVER_HPP

#include <NonConstVisitor.hpp>

namespace Lox
{
    class Resolver : public AST::NonConstVisitor
    {
    public:
        Resolver();
        ~Resolver();
        
        void visitExpressionStmt(AST::ExpressionStmt& stmt);
        void visitVarDeclStmt(AST::VarDeclStmt& stmt);
        void visitIfStmt(AST::IfStmt& stmt);
        void visitWhileStmt(AST::WhileStmt& stmt);
        void visitDoWhileStmt(AST::DoWhileStmt& stmt);
        void visitForStmt(AST::ForStmt& stmt);
        void visitBreakStmt(AST::BreakStmt& stmt);
        void visitContinueStmt(AST::ContinueStmt& stmt);
        void visitPrintStmt(AST::PrintStmt& stmt);
        void visitReturnStmt(AST::ReturnStmt& stmt);
        
        void visitLiteralExpr(AST::LiteralExpr& expr);
        void visitVarExpr(AST::VarExpr& expr);
        void visitGroupExpr(AST::GroupExpr& expr);
        void visitUnaryExpr(AST::UnaryExpr& expr);
        void visitPostIncrementExpr(AST::PostIncrementExpr& expr);
        void visitPreIncrementExpr(AST::PreIncrementExpr& expr);
        void visitBinaryExpr(AST::BinaryExpr& expr);
        void visitTernaryExpr(AST::TernaryExpr& expr);
        void visitAssignExpr(AST::AssignExpr& expr);
        void visitLogicalExpr(AST::LogicalExpr& expr);
        void visitGetExpr(AST::GetExpr& expr);
        void visitSetExpr(AST::SetExpr& expr);
        void visitIndexExpr(AST::IndexExpr& expr);
        void visitSuperExpr(AST::SuperExpr& expr);
    private:
        void resolve(AST::Stmt stmt);
        void resolve(AST::Expr expr);
    }; // class Resolver
} // namespace Lox

#endif // LOX_RESOLVER_HPP
