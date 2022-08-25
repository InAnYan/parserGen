#ifndef LOX_NON_CONST_VISITOR_HPP
#define LOX_NON_CONST_VISITOR_HPP

#include <ASTForwards.hpp>

namespace Lox
{
    namespace AST
    {
        class NonConstVisitor
        {
        public:
            virtual ~NonConstVisitor() {}
            
            virtual void visitExpressionStmt(ExpressionStmt& stmt) = 0;
            virtual void visitVarDeclStmt(VarDeclStmt& stmt) = 0;
            virtual void visitIfStmt(IfStmt& stmt) = 0;
            virtual void visitWhileStmt(WhileStmt& stmt) = 0;
            virtual void visitDoWhileStmt(DoWhileStmt& stmt) = 0;
            virtual void visitForStmt(ForStmt& stmt) = 0;
            virtual void visitBreakStmt(BreakStmt& stmt) = 0;
            virtual void visitContinueStmt(ContinueStmt& stmt) = 0;
            virtual void visitPrintStmt(PrintStmt& stmt) = 0;
            virtual void visitReturnStmt(ReturnStmt& stmt) = 0;
            
            virtual void visitLiteralExpr(LiteralExpr& expr) = 0;
            virtual void visitVarExpr(VarExpr& expr) = 0;
            virtual void visitGroupExpr(GroupExpr& expr) = 0;
            virtual void visitUnaryExpr(UnaryExpr& expr) = 0;
            virtual void visitPostIncrementExpr(PostIncrementExpr& expr) = 0;
            virtual void visitPreIncrementExpr(PreIncrementExpr& expr) = 0;
            virtual void visitBinaryExpr(BinaryExpr& expr) = 0;
            virtual void visitTernaryExpr(TernaryExpr& expr) = 0;
            virtual void visitAssignExpr(AssignExpr& expr) = 0;
            virtual void visitLogicalExpr(LogicalExpr& expr) = 0;
            virtual void visitGetExpr(GetExpr& expr) = 0;
            virtual void visitSetExpr(SetExpr& expr) = 0;
            virtual void visitIndexExpr(IndexExpr& expr) = 0;
            virtual void visitSuperExpr(SuperExpr& expr) = 0;
        }; // class NonConstVisitor
    } // namespace AST
} // namespace Lox

#endif // LOX_NON_CONST_VISITOR_HPP
