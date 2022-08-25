        
        #ifndef LOX_NON_CONST_VISITOR_HPP
        #define LOX_NON_CONST_VISITOR_HPP
        
        #include <ASTForwards.hpp>
        
        namespace Lox
        {
            class NonConstVisitor
            {
            public:
                virtual ~NonConstVisitor() {}
                
                virtual void visitExpressionStmt(ExpressionStmt& stmt);
                virtual void visitVarDeclStmt(VarDeclStmt& stmt);
                virtual void visitIfStmt(IfStmt& stmt);
                virtual void visitWhileStmt(WhileStmt& stmt);
                virtual void visitDoWhileStmt(DoWhileStmt& stmt);
                virtual void visitForStmt(ForStmt& stmt);
                virtual void visitBreakStmt(BreakStmt& stmt);
                virtual void visitContinueStmt(ContinueStmt& stmt);
                virtual void visitPrintStmt(PrintStmt& stmt);
                virtual void visitReturnStmt(ReturnStmt& stmt);
                
                virtual void visitLiteralExpr(LiteralExpr& expr);
                virtual void visitVarExpr(VarExpr& expr);
                virtual void visitGroupExpr(GroupExpr& expr);
                virtual void visitUnaryExpr(UnaryExpr& expr);
                virtual void visitPostIncrementExpr(PostIncrementExpr& expr);
                virtual void visitPreIncrementExpr(PreIncrementExpr& expr);
                virtual void visitBinaryExpr(BinaryExpr& expr);
                virtual void visitTernaryExpr(TernaryExpr& expr);
                virtual void visitAssignExpr(AssignExpr& expr);
                virtual void visitLogicalExpr(LogicalExpr& expr);
                virtual void visitGetExpr(GetExpr& expr);
                virtual void visitSetExpr(SetExpr& expr);
                virtual void visitIndexExpr(IndexExpr& expr);
                virtual void visitSuperExpr(SuperExpr& expr);
            };
        } // Lox
        
        #endif // LOX_NON_CONST_VISITOR_HPP
