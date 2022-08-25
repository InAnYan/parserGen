#ifndef LOX_GRAPHDOT_HPP
#define LOX_GRAPHDOT_HPP

#include <Visitor.hpp>

namespace Lox
{
    class GraphDot : public AST::Visitor
    {
    public:
        GraphDot();
        ~GraphDot();
        
        void visitExpressionStmt(const AST::ExpressionStmt& stmt);
        void visitVarDeclStmt(const AST::VarDeclStmt& stmt);
        void visitIfStmt(const AST::IfStmt& stmt);
        void visitWhileStmt(const AST::WhileStmt& stmt);
        void visitDoWhileStmt(const AST::DoWhileStmt& stmt);
        void visitForStmt(const AST::ForStmt& stmt);
        void visitBreakStmt(const AST::BreakStmt& stmt);
        void visitContinueStmt(const AST::ContinueStmt& stmt);
        void visitPrintStmt(const AST::PrintStmt& stmt);
        void visitReturnStmt(const AST::ReturnStmt& stmt);
        
        void visitLiteralExpr(const AST::LiteralExpr& expr);
        void visitVarExpr(const AST::VarExpr& expr);
        void visitGroupExpr(const AST::GroupExpr& expr);
        void visitUnaryExpr(const AST::UnaryExpr& expr);
        void visitPostIncrementExpr(const AST::PostIncrementExpr& expr);
        void visitPreIncrementExpr(const AST::PreIncrementExpr& expr);
        void visitBinaryExpr(const AST::BinaryExpr& expr);
        void visitTernaryExpr(const AST::TernaryExpr& expr);
        void visitAssignExpr(const AST::AssignExpr& expr);
        void visitLogicalExpr(const AST::LogicalExpr& expr);
        void visitGetExpr(const AST::GetExpr& expr);
        void visitSetExpr(const AST::SetExpr& expr);
        void visitIndexExpr(const AST::IndexExpr& expr);
        void visitSuperExpr(const AST::SuperExpr& expr);
    private:
        void draw(const AST::Stmt stmt);
        void draw(const AST::Expr expr);
    }; // class GraphDot
} // namespace Lox

#endif // LOX_GRAPHDOT_HPP