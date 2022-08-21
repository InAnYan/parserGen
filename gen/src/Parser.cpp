#include <pscpch.hpp>
#include <Parser.hpp>

namespace Pascal
{
    Parser::Parser(TokenList tokens)
        : m_Tokens(tokens)
    { }
    
    std::unique_ptr<AST::programNode> Parser::parseprogram()
    {
        std::vector<std::unique_ptr<AST::declarationNode>> t_decls;
        
        while (!matching(TokenType::EOF))
        {
            t_decls.push_back(parsedeclaration());
        }
        
        return std::make_unique<AST::programNode>(
            std::move(t_decls)
        );
    }
    
    std::unique_ptr<AST::declarationNode> Parser::parsedeclaration()
    {
        std::unique_ptr<AST::declarationNode> t_declaration = nullptr;
        
        if (matching(TokenType::VAR))
        {
            t_declaration = parsevarDecl();
        }
        else
        {
            t_declaration = parsestatement();
        }
        
        return std::move(t_declaration);
    }
    
    std::unique_ptr<AST::varDeclNode> Parser::parsevarDecl()
    {
        Token_t t_name;
        std::unique_ptr<AST::expressionNode> t_initializer;
        
        t_name = require(TokenType::IDENTIFIER);
        if (matching(TokenType::EQUALS))
        {
            t_initializer = parseexpression();
        }
        
        return std::make_unique<AST::varDeclNode>(
            std::move(t_name), 
            std::move(t_initializer)
        );
    }
    
    std::unique_ptr<AST::statementNode> Parser::parsestatement()
    {
        std::unique_ptr<AST::statementNode> t_statement = nullptr;
        
        if (matching(TokenType::PRINT))
        {
            t_statement = parseprintStmt();
        }
        else if (matching(TokenType::OPEN_FIGURE))
        {
            t_statement = parseblockStmt();
        }
        else if (matching(TokenType::IF))
        {
            t_statement = parseifStmt();
        }
        else
        {
            t_statement = parseexprStatement();
        }
        
        return std::move(t_statement);
    }
    
    std::unique_ptr<AST::printStmtNode> Parser::parseprintStmt()
    {
        std::unique_ptr<AST::expressionNode> t_expr;
        
        t_expr = parseexpression();
        require(TokenType::SEMICOLON);
        
        return std::make_unique<AST::printStmtNode>(
            std::move(t_expr)
        );
    }
    
    std::unique_ptr<AST::exprStatementNode> Parser::parseexprStatement()
    {
        std::unique_ptr<AST::expressionNode> t_expr;
        
        t_expr = parseexpression();
        require(TokenType::SEMICOLON);
        
        return std::make_unique<AST::exprStatementNode>(
            std::move(t_expr)
        );
    }
    
    std::unique_ptr<AST::blockStmtNode> Parser::parseblockStmt()
    {
        std::vector<std::unique_ptr<AST::declarationNode>> t_decls;
        
        while (!matching(TokenType::CLOSE_FIGURE))
        {
            t_decls.push_back(parsedeclaration());
        }
        
        return std::make_unique<AST::blockStmtNode>(
            std::move(t_decls)
        );
    }
    
    std::unique_ptr<AST::ifStmtNode> Parser::parseifStmt()
    {
        std::unique_ptr<AST::expressionNode> t_condition;
        std::unique_ptr<AST::statementNode> t_trueStmt;
        std::unique_ptr<AST::statementNode> t_falseStmt;
        
        require(TokenType::OPEN_PAREN);
        t_condition = parseexpression();
        require(TokenType::CLOSE_PAREN);
        t_trueStmt = parsestatement();
        if (matching(TokenType::ELSE))
        {
            t_falseStmt = parsestatement();
        }
        
        return std::make_unique<AST::ifStmtNode>(
            std::move(t_condition), 
            std::move(t_trueStmt), 
            std::move(t_falseStmt)
        );
    }
    
    std::unique_ptr<AST::expressionNode> Parser::parseexpression()
    {
        std::unique_ptr<AST::expressionNode> t_expression = nullptr;
        
        t_expression = parseexpressionComma();
        
        return std::move(t_expression);
    }
    
    std::unique_ptr<AST::expressionCommaNode> Parser::parseexpressionComma()
    {
        std::unique_ptr<AST::NodeNode> left = parseassignment;
        
        while (matching(TokenType::TokenType::COMMA))
        {
            Token op = previousToken();
            std::unique_ptr<AST::NodeNode> right = parseassignment;
            left = std::make_unique<AST::expressionCommaNode>(std::move(left), op, std::move(right);
        }
        
        return left;
        
        return std::make_unique<AST::expressionCommaNode>(
        );
    }
    
    std::unique_ptr<AST::assignmentNode> Parser::parseassignment()
    {
        if (currentToken().type == TokenType::PLUS || currentToken().type == TokenType::MINUS || currentToken().type == TokenType::BANG || currentToken().type == TokenType::INT_LITERAL || currentToken().type == TokenType::IDENTIFIER || currentToken().type == TokenType::TRUE || currentToken().type == TokenType::FALSE || currentToken().type == TokenType::OPEN_PAREN)
        {
            parselogic_or();
        }
        else
        {
            parselogic_or();
            require(TokenType::EQUALS);
            parseassignment();
        }
        
        return std::make_unique<AST::assignmentNode>(
        );
    }
    
    std::unique_ptr<AST::logic_orNode> Parser::parselogic_or()
    {
        parselogic_and();
        require(TokenType::OR);
        parselogic_and();
        
        return std::make_unique<AST::logic_orNode>(
        );
    }
    
    std::unique_ptr<AST::logic_andNode> Parser::parselogic_and()
    {
        parseequality();
        require(TokenType::AND);
        parseequality();
        
        return std::make_unique<AST::logic_andNode>(
        );
    }
    
    std::unique_ptr<AST::equalityNode> Parser::parseequality()
    {
        parseternary();
        if (currentToken().type == TokenType::EQUALS_EQUALS)
        {
            require(TokenType::EQUALS_EQUALS);
        }
        else
        {
            require(TokenType::BANG_EQUALS);
        }
        parseternary();
        
        return std::make_unique<AST::equalityNode>(
        );
    }
    
    std::unique_ptr<AST::ternaryNode> Parser::parseternary()
    {
        parsecomparison();
        require(TokenType::QUESTION);
        parsecomparison();
        require(TokenType::COLON);
        parseternary();
        
        return std::make_unique<AST::ternaryNode>(
        );
    }
    
    std::unique_ptr<AST::comparisonNode> Parser::parsecomparison()
    {
        parseterm();
        if (currentToken().type == TokenType::GREATER_EQUALS)
        {
            require(TokenType::GREATER_EQUALS);
        }
        else if (currentToken().type == TokenType::GREATER)
        {
            require(TokenType::GREATER);
        }
        else if (currentToken().type == TokenType::LESS)
        {
            require(TokenType::LESS);
        }
        else
        {
            require(TokenType::LESS_EQUALS);
        }
        parseterm();
        
        return std::make_unique<AST::comparisonNode>(
        );
    }
    
    std::unique_ptr<AST::termNode> Parser::parseterm()
    {
        parsefactor();
        if (currentToken().type == TokenType::PLUS)
        {
            require(TokenType::PLUS);
        }
        else
        {
            require(TokenType::MINUS);
        }
        parsefactor();
        
        return std::make_unique<AST::termNode>(
        );
    }
    
    std::unique_ptr<AST::factorNode> Parser::parsefactor()
    {
        parseunary();
        if (currentToken().type == TokenType::ASTERISK)
        {
            require(TokenType::ASTERISK);
        }
        else if (currentToken().type == TokenType::DIVISION)
        {
            require(TokenType::DIVISION);
        }
        else
        {
            require(TokenType::PERCENT);
        }
        parseunary();
        
        return std::make_unique<AST::factorNode>(
        );
    }
    
    std::unique_ptr<AST::unaryNode> Parser::parseunary()
    {
        if (currentToken().type == TokenType::PLUS || currentToken().type == TokenType::MINUS || currentToken().type == TokenType::BANG)
        {
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
                require(TokenType::BANG);
            }
            parseunary();
        }
        else
        {
            parseprimary();
        }
        
        return std::make_unique<AST::unaryNode>(
        );
    }
    
    std::unique_ptr<AST::primaryNode> Parser::parseprimary()
    {
        if (currentToken().type == TokenType::INT_LITERAL)
        {
            require(TokenType::INT_LITERAL);
        }
        else if (currentToken().type == TokenType::IDENTIFIER)
        {
            require(TokenType::IDENTIFIER);
        }
        else if (currentToken().type == TokenType::TRUE)
        {
            require(TokenType::TRUE);
        }
        else if (currentToken().type == TokenType::FALSE)
        {
            require(TokenType::FALSE);
        }
        else
        {
            require(TokenType::OPEN_PAREN);
            parseexpression();
            require(TokenType::CLOSE_PAREN);
        }
        
        return std::make_unique<AST::primaryNode>(
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

