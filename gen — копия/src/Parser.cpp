#include <pscpch.hpp>
#include <Parser.hpp>

namespace Pascal
{
    Parser::Parser(TokenList tokens)
        : m_Tokens(tokens)
    { }
    
    std::unique_ptr<AST::ProgramNode> Parser::parseProgram()
    {
        Token_t t_Name;
        std::unique_ptr<AST::BlockNode> t_Block;
        
        require(TokenType::PROGRAM);
        t_Name = require(TokenType::IDENTIFIER);
        require(TokenType::SEMICOLON);
        std::unique_ptr<AST::BlockNode> t_Block = parseBlock();
        require(TokenType::DOT);
        
        return std::make_unique<AST::ProgramNode>(
            std::move(t_Name), 
            std::move(t_Block)
        );
    }
    
    std::unique_ptr<AST::BlockNode> Parser::parseBlock()
    {
        std::vector<std::unique_ptr<AST::DeclarationNode>> t_Decls;
        std::unique_ptr<AST::CompoundNode> t_Compound;
        
        while (currentToken().type == TokenType::VAR || currentToken().type == TokenType::PROCEDURE)
        {
            t_Decls.push_back(parseDeclaration());
        }
        std::unique_ptr<AST::CompoundNode> t_Compound = parseCompound();
        
        return std::make_unique<AST::BlockNode>(
            std::move(t_Decls), 
            std::move(t_Compound)
        );
    }
    
    std::unique_ptr<AST::DeclarationNode> Parser::parseDeclaration()
    {
        std::unique_ptr<AST::DeclarationNode> t_Declaration = nullptr;
        
        if (currentToken().type == TokenType::VAR)
        {
            t_Declaration = parseVarDecl();
        }
        else if (currentToken().type == TokenType::PROCEDURE)
        {
            t_Declaration = parseProcDecl();
        }
        else
        {
            // TODO: Error
        }
        require(TokenType::SEMICOLON);
        
        return std::move(t_Declaration);
    }
    
    std::unique_ptr<AST::VarDeclNode> Parser::parseVarDecl()
    {
        std::vector<Token_t> t_Ids;
        Token_t t_Type;
        
        require(TokenType::VAR);
        do
        {
            t_Ids.push_back(require(TokenType::IDENTIFIER));
        }
        while (currentToken().type == TokenType::IDENTIFIER);
        require(TokenType::COLON);
        t_Type = require(TokenType::IDENTIFIER);
        
        return std::make_unique<AST::VarDeclNode>(
            std::move(t_Ids), 
            std::move(t_Type)
        );
    }
    
    std::unique_ptr<AST::ProcDeclNode> Parser::parseProcDecl()
    {
        Token_t t_Name;
        std::unique_ptr<AST::BlockNode> t_Block;
        
        require(TokenType::PROCEDURE);
        t_Name = require(TokenType::IDENTIFIER);
        if (matching(TokenType::OPEN_PAREN))
        {
            require(TokenType::CLOSE_PAREN);
        }
        require(TokenType::SEMICOLON);
        std::unique_ptr<AST::BlockNode> t_Block = parseBlock();
        
        return std::make_unique<AST::ProcDeclNode>(
            std::move(t_Name), 
            std::move(t_Block)
        );
    }
    
    std::unique_ptr<AST::CompoundNode> Parser::parseCompound()
    {
        std::vector<std::unique_ptr<AST::StatementNode>> t_Statements;
        
        require(TokenType::BEGIN);
        while (!matching(TokenType::END))
        {
            t_Statements.push_back(parseStatement());
        }
        
        return std::make_unique<AST::CompoundNode>(
            std::move(t_Statements)
        );
    }
    
    std::unique_ptr<AST::StatementNode> Parser::parseStatement()
    {
        std::unique_ptr<AST::StatementNode> t_Statement = nullptr;
        
        if (currentToken().type == TokenType::BEGIN)
        {
            t_Statement = parseCompoundStatement();
        }
        else if (currentToken().type == TokenType::IDENTIFIER)
        {
            t_Statement = parseAssignmentStatement();
        }
        else if (currentToken().type == TokenType::IDENTIFIER)
        {
            t_Statement = parseProcCallStatement();
        }
        else
        {
            // TODO: Error
        }
        require(TokenType::SEMICOLON);
        
        return std::move(t_Statement);
    }
    
    std::unique_ptr<AST::CompoundStatementNode> Parser::parseCompoundStatement()
    {
        std::vector<std::unique_ptr<AST::StatementNode>> t_Statements;
        
        require(TokenType::BEGIN);
        while (!matching(TokenType::END))
        {
            t_Statements.push_back(parseStatement());
        }
        
        return std::make_unique<AST::CompoundStatementNode>(
            std::move(t_Statements)
        );
    }
    
    std::unique_ptr<AST::AssignmentStatementNode> Parser::parseAssignmentStatement()
    {
        Token_t t_Var;
        std::unique_ptr<AST::ExprNode> t_Expr;
        
        t_Var = require(TokenType::IDENTIFIER);
        require(TokenType::COLON_EQUALS);
        std::unique_ptr<AST::ExprNode> t_Expr = parseExpr();
        
        return std::make_unique<AST::AssignmentStatementNode>(
            std::move(t_Var), 
            std::move(t_Expr)
        );
    }
    
    std::unique_ptr<AST::ProcCallStatementNode> Parser::parseProcCallStatement()
    {
        Token_t t_ProcName;
        std::vector<std::unique_ptr<AST::ExprNode>> t_Args;
        
        t_ProcName = require(TokenType::IDENTIFIER);
        require(TokenType::OPEN_PAREN);
        while (!matching(TokenType::CLOSE_PAREN))
        {
            t_Args.push_back(parseExpr());
        }
        
        return std::make_unique<AST::ProcCallStatementNode>(
            std::move(t_ProcName), 
            std::move(t_Args)
        );
    }
    
    std::unique_ptr<AST::ExprNode> Parser::parseExpr()
    {
        if (currentToken().type == TokenType::INT_LITERAL || currentToken().type == TokenType::FLOAT_LITERAL || currentToken().type == TokenType::PLUS || currentToken().type == TokenType::MINUS || currentToken().type == TokenType::INT_LITERAL || currentToken().type == TokenType::FLOAT_LITERAL || currentToken().type == TokenType::PLUS || currentToken().type == TokenType::MINUS)
        {
            parseTerm();
        }
        else if (currentToken().type == TokenType::INT_LITERAL || currentToken().type == TokenType::FLOAT_LITERAL || currentToken().type == TokenType::PLUS || currentToken().type == TokenType::MINUS || currentToken().type == TokenType::INT_LITERAL || currentToken().type == TokenType::FLOAT_LITERAL || currentToken().type == TokenType::PLUS || currentToken().type == TokenType::MINUS)
        {
            parseTerm();
            if (currentToken().type == TokenType::PLUS)
            {
                require(TokenType::PLUS);
            }
            else if (currentToken().type == TokenType::MINUS)
            {
                require(TokenType::MINUS);
            }
            else
            {
                // TODO: Error
            }
            parseExpr();
        }
        else
        {
            // TODO: Error
        }
        
        return std::make_unique<AST::ExprNode>(
        );
    }
    
    std::unique_ptr<AST::TermNode> Parser::parseTerm()
    {
        if (currentToken().type == TokenType::INT_LITERAL || currentToken().type == TokenType::FLOAT_LITERAL || currentToken().type == TokenType::PLUS || currentToken().type == TokenType::MINUS)
        {
            parseFactor();
        }
        else if (currentToken().type == TokenType::INT_LITERAL || currentToken().type == TokenType::FLOAT_LITERAL || currentToken().type == TokenType::PLUS || currentToken().type == TokenType::MINUS)
        {
            parseFactor();
            if (currentToken().type == TokenType::DIVISION)
            {
                require(TokenType::DIVISION);
            }
            else if (currentToken().type == TokenType::ASTERISK)
            {
                require(TokenType::ASTERISK);
            }
            else if (currentToken().type == TokenType::MOD)
            {
                require(TokenType::MOD);
            }
            else
            {
                // TODO: Error
            }
            parseTerm();
        }
        else
        {
            // TODO: Error
        }
        
        return std::make_unique<AST::TermNode>(
        );
    }
    
    std::unique_ptr<AST::FactorNode> Parser::parseFactor()
    {
        if (currentToken().type == TokenType::INT_LITERAL)
        {
            require(TokenType::INT_LITERAL);
        }
        else if (currentToken().type == TokenType::FLOAT_LITERAL)
        {
            require(TokenType::FLOAT_LITERAL);
        }
        else if (currentToken().type == TokenType::PLUS)
        {
            require(TokenType::PLUS);
            parseFactor();
        }
        else if (currentToken().type == TokenType::MINUS)
        {
            require(TokenType::MINUS);
            parseFactor();
        }
        else
        {
            // TODO: Error
        }
        
        return std::make_unique<AST::FactorNode>(
        );
    }
    
    Token Parser::match(TokenType type)
    {
        Token curTok = currentToken();
        if (curTok.type == type)
        {
            m_ParserPos++;
            return curTok;
        }
        else return nullToken;
    }
    
    Token Parser::match(TokenType type)
    {
        return match(type).type != TokenType::NONE;
    }
    
    Token Parser::require(TokenType type)
    {
        Token curTok = match(TokenType::type);
        if (curTok.type != TokenType::NONE)
        {
            return curTok;
        }
        // TODO: else error();
    }
    
    Token Parser::currentToken(size_t offset)
    {
        if (m_ParserPos < m_Tokens->size())
        {
            return (*m_Tokens)[m_ParserPos];
        }
        else return nullToken;
    }
    
} // Pascal

