#ifndef LOX_PARSER_HPP_DEFINED
#define LOX_PARSER_HPP_DEFINED

#include <memory>

#include <Lexer.hpp>
#include <AST.hpp>

namespace Lox
{
    class Parser
    {
    public:
        Parser(TokenList tokens);
        
    private:
        std::unique_ptr<AST::ProgramNode> parseProgram();
        std::unique_ptr<AST::DeclarationNode> parseDeclaration();
        std::unique_ptr<AST::VarDeclNode> parseVarDecl();
        std::unique_ptr<AST::StatementNode> parseStatement();
        std::unique_ptr<AST::PrintStmtNode> parsePrintStmt();
        std::unique_ptr<AST::ExprStmtNode> parseExprStmt();
        std::unique_ptr<AST::BlockStmtNode> parseBlockStmt();
        std::unique_ptr<AST::IfStmtNode> parseIfStmt();
        std::unique_ptr<AST::ExpressionNode> parseExpression();
        std::unique_ptr<AST::ExpressionCommaNode> parseExpressionComma();
        std::unique_ptr<AST::AssignmentExprNode> parseAssignmentExpr();
        std::unique_ptr<AST::TernaryExprNode> parseTernaryExpr();
        std::unique_ptr<AST::LogicOrExprNode> parseLogicOrExpr();
        std::unique_ptr<AST::LogicAndExprNode> parseLogicAndExpr();
        std::unique_ptr<AST::EqualityExprNode> parseEqualityExpr();
        std::unique_ptr<AST::ComparisonExprNode> parseComparisonExpr();
        std::unique_ptr<AST::TermExprNode> parseTermExpr();
        std::unique_ptr<AST::FactorExprNode> parseFactorExpr();
        std::unique_ptr<AST::UnaryExprNode> parseUnaryExpr();
        std::unique_ptr<AST::PrimaryExprNode> parsePrimaryExpr();
        
        TokenList m_Tokens;
        size_t m_ParserPos;
        const Token_t nullToken = { TokenType::NONE, "", 0 };
        
        Token require(TokenType type);
        Token match(TokenType type);
        bool matching(TokenType type);
        
        Token currentToken(size_t offset = 0);
    } // Parser
} // Lox

#endif // LOX_PARSER_HPP_DEFINED

