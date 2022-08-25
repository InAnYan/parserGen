#ifndef LOX_AST_HPP_INCLUDED
#define LOX_AST_HPP_INCLUDED

#include <memory>
struct Token_t {};
namespace Lox
{
    namespace AST
    {
        class Visitor;
        class NonConstVisitor;
        
        class Node
        {
        public:
            virtual ~Node() {}
            virtual void accept(Visitor* visitor) const = 0;
            virtual void accept(NonConstVisitor* visitor) = 0;
        };
        
        #ifndef LOX_AST_FORWARDS_HPP
        #define LOX_AST_FORWARDS_HPP
        
        namespace Lox
        {
            // Forward declarations
            
            class Stmt;
            class ExpressionStmt;
            class VarDeclStmt;
            class IfStmt;
            class WhileStmt;
            class DoWhileStmt;
            class ForStmt;
            class BreakStmt;
            class ContinueStmt;
            class PrintStmt;
            class ReturnStmt;
            
            class Expr;
            class LiteralExpr;
            class VarExpr;
            class GroupExpr;
            class UnaryExpr;
            class PostIncrementExpr;
            class PreIncrementExpr;
            class BinaryExpr;
            class TernaryExpr;
            class AssignExpr;
            class LogicalExpr;
            class GetExpr;
            class SetExpr;
            class IndexExpr;
            class SuperExpr;
        } // Lox
        
        #endif // LOX_AST_FORWARDS_HPP
