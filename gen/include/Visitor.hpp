#ifndef LOX_VISITOR_HPP
#define LOX_VISITOR_HPP

#include <ASTForwards.hpp>

namespace Lox
{
    namespace AST
    {
        class Visitor
        {
        public:
            virtual ~Visitor() {}
            
            virtual void visitExpressionStmt(const AST::ExpressionStmt& stmt) = 0;
            virtual void visitVarDeclStmt(const AST::VarDeclStmt& stmt) = 0;
            virtual void visitIfStmt(const AST::IfStmt& stmt) = 0;
            virtual void visitWhileStmt(const AST::WhileStmt& stmt) = 0;
            virtual void visitDoWhileStmt(const AST::DoWhileStmt& stmt) = 0;
            virtual void visitForStmt(const AST::ForStmt& stmt) = 0;
            virtual void visitBreakStmt(const AST::BreakStmt& stmt) = 0;
            virtual void visitContinueStmt(const AST::ContinueStmt& stmt) = 0;
            virtual void visitPrintStmt(const AST::PrintStmt& stmt) = 0;
            virtual void visitReturnStmt(const AST::ReturnStmt& stmt) = 0;
            
            virtual void visitLiteralExpr(const AST::LiteralExpr& expr) = 0;
            virtual void visitVarExpr(const AST::VarExpr& expr) = 0;
            virtual void visitGroupExpr(const AST::GroupExpr& expr) = 0;
            virtual void visitUnaryExpr(const AST::UnaryExpr& expr) = 0;
            virtual void visitPostIncrementExpr(const AST::PostIncrementExpr& expr) = 0;
            virtual void visitPreIncrementExpr(const AST::PreIncrementExpr& expr) = 0;
            virtual void visitBinaryExpr(const AST::BinaryExpr& expr) = 0;
            virtual void visitTernaryExpr(const AST::TernaryExpr& expr) = 0;
            virtual void visitAssignExpr(const AST::AssignExpr& expr) = 0;
            virtual void visitLogicalExpr(const AST::LogicalExpr& expr) = 0;
            virtual void visitGetExpr(const AST::GetExpr& expr) = 0;
            virtual void visitSetExpr(const AST::SetExpr& expr) = 0;
            virtual void visitIndexExpr(const AST::IndexExpr& expr) = 0;
            virtual void visitSuperExpr(const AST::SuperExpr& expr) = 0;
        }; // class Visitor
    } // namespace AST
} // namespace Lox

#endif // LOX_VISITOR_HPP
