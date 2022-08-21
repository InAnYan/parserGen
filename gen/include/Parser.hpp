#ifndef PASCAL_PARSER_HPP_DEFINED
#define PASCAL_PARSER_HPP_DEFINED

#include <memory>

#include <Lexer.hpp>
#include <AST.hpp>

namespace Pascal
{
    class Parser
    {
    public:
        Parser(TokenList tokens);
        
        std::unique_ptr<AST::programNode> parseprogram();
        std::unique_ptr<AST::declarationNode> parsedeclaration();
        std::unique_ptr<AST::varDeclNode> parsevarDecl();
        std::unique_ptr<AST::statementNode> parsestatement();
        std::unique_ptr<AST::printStmtNode> parseprintStmt();
        std::unique_ptr<AST::exprStatementNode> parseexprStatement();
        std::unique_ptr<AST::blockStmtNode> parseblockStmt();
        std::unique_ptr<AST::ifStmtNode> parseifStmt();
        std::unique_ptr<AST::expressionNode> parseexpression();
        std::unique_ptr<AST::expressionCommaNode> parseexpressionComma();
        std::unique_ptr<AST::assignmentNode> parseassignment();
        std::unique_ptr<AST::logic_orNode> parselogic_or();
        std::unique_ptr<AST::logic_andNode> parselogic_and();
        std::unique_ptr<AST::equalityNode> parseequality();
        std::unique_ptr<AST::ternaryNode> parseternary();
        std::unique_ptr<AST::comparisonNode> parsecomparison();
        std::unique_ptr<AST::termNode> parseterm();
        std::unique_ptr<AST::factorNode> parsefactor();
        std::unique_ptr<AST::unaryNode> parseunary();
        std::unique_ptr<AST::primaryNode> parseprimary();
        
    private:
        TokenList m_Tokens;
        size_t m_ParserPos;
        Token_t nullToken = { TokenType::NONE, "", 0 };
        
        Token Parser::require(TokenType type);
        Token Parser::match(TokenType type);
        bool Parser::matching(TokenType type);
        
        Token currentToken(size_t offset = 0);
    } // Parser
} // Pascal

#endif // PASCAL_PARSER_HPP_DEFINED

