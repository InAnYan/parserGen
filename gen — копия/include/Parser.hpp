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
        
        std::unique_ptr<AST::ProgramNode> parseProgram();
        std::unique_ptr<AST::BlockNode> parseBlock();
        std::unique_ptr<AST::DeclarationNode> parseDeclaration();
        std::unique_ptr<AST::VarDeclNode> parseVarDecl();
        std::unique_ptr<AST::ProcDeclNode> parseProcDecl();
        std::unique_ptr<AST::CompoundNode> parseCompound();
        std::unique_ptr<AST::StatementNode> parseStatement();
        std::unique_ptr<AST::CompoundStatementNode> parseCompoundStatement();
        std::unique_ptr<AST::AssignmentStatementNode> parseAssignmentStatement();
        std::unique_ptr<AST::ProcCallStatementNode> parseProcCallStatement();
        std::unique_ptr<AST::ExprNode> parseExpr();
        std::unique_ptr<AST::TermNode> parseTerm();
        std::unique_ptr<AST::FactorNode> parseFactor();
        
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

