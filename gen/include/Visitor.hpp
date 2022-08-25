        
        #ifndef LOX_VISITOR_HPP
        #define LOX_VISITOR_HPP
        
        #include <ASTForwards.hpp>
        
        namespace Lox
        {
            class Visitor
            {
            public:
                virtual ~Visitor() {}
                
                virtual void visitExpressionStmt(const ExpressionStmt& stmt);
                virtual void visitVarDeclStmt(const VarDeclStmt& stmt);
                virtual void visitIfStmt(const IfStmt& stmt);
                virtual void visitWhileStmt(const WhileStmt& stmt);
                virtual void visitDoWhileStmt(const DoWhileStmt& stmt);
                virtual void visitForStmt(const ForStmt& stmt);
                virtual void visitBreakStmt(const BreakStmt& stmt);
                virtual void visitContinueStmt(const ContinueStmt& stmt);
                virtual void visitPrintStmt(const PrintStmt& stmt);
                virtual void visitReturnStmt(const ReturnStmt& stmt);
                
                virtual void visitLiteralExpr(const LiteralExpr& expr);
                virtual void visitVarExpr(const VarExpr& expr);
                virtual void visitGroupExpr(const GroupExpr& expr);
                virtual void visitUnaryExpr(const UnaryExpr& expr);
                virtual void visitPostIncrementExpr(const PostIncrementExpr& expr);
                virtual void visitPreIncrementExpr(const PreIncrementExpr& expr);
                virtual void visitBinaryExpr(const BinaryExpr& expr);
                virtual void visitTernaryExpr(const TernaryExpr& expr);
                virtual void visitAssignExpr(const AssignExpr& expr);
                virtual void visitLogicalExpr(const LogicalExpr& expr);
                virtual void visitGetExpr(const GetExpr& expr);
                virtual void visitSetExpr(const SetExpr& expr);
                virtual void visitIndexExpr(const IndexExpr& expr);
                virtual void visitSuperExpr(const SuperExpr& expr);
            };
        } // Lox
        
        #endif // LOX_VISITOR_HPP
