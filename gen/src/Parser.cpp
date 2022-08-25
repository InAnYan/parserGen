#include <pscpch.hpp>
#include <Parser.hpp>

namespace Lox
{
    Parser::Parser(TokenList tokens)
        : m_Tokens(tokens)
    { }
    
    std::unique_ptr<AST::ProgramNode> Parser::parseProgram()
    {
        std::vector<std::unique_ptr<AST::DeclarationNode>> t_Decls;
        
        while (!matching(TokenType::EOF))
        {
            t_Decls.push_back(parseDeclaration());
        }
        
        return std::make_unique<AST::ProgramNode>(
            std::move(t_Decls)
        );
    }
    
    std::unique_ptr<AST::DeclarationNode> Parser::parseDeclaration()
    {
        std::unique_ptr<AST::DeclarationNode> t_Declaration = nullptr;
        
        if (currentToken().type == TokenType::VAR)
        {
            t_Declaration = parseVarDecl();
        }
        else
        {
            t_Declaration = parseStatement();
        }
        
        return std::move(t_Declaration);
    }
    
    std::unique_ptr<AST::VarDeclNode> Parser::parseVarDecl()
    {
        Token_t t_Name;
        std::unique_ptr<AST::ExpressionNode> t_Initializer;
        
        require(TokenType::VAR);
        t_Name = require(TokenType::IDENTIFIER);
        if (matching(TokenType::EQUALS))
        {
            t_Initializer = parseExpression();
        }
        
        return std::make_unique<AST::VarDeclNode>(
            std::move(t_Name), 
            std::move(t_Initializer)
        );
    }
    
    std::unique_ptr<AST::StatementNode> Parser::parseStatement()
    {
        std::unique_ptr<AST::StatementNode> t_Statement = nullptr;
        
        if (currentToken().type == TokenType::PRINT)
        {
            t_Statement = parsePrintStmt();
        }
        else if (currentToken().type == TokenType::OPEN_FIGURE)
        {
            t_Statement = parseBlockStmt();
        }
        else if (currentToken().type == TokenType::IF)
        {
            t_Statement = parseIfStmt();
        }
        else
        {
            t_Statement = parseExprStmt();
        }
        
        return std::move(t_Statement);
    }
    
    std::unique_ptr<AST::PrintStmtNode> Parser::parsePrintStmt()
    {
        std::unique_ptr<AST::ExpressionNode> t_Expr;
        
        require(TokenType::PRINT);
        t_Expr = parseExpression();
        require(TokenType::SEMICOLON);
        
        return std::make_unique<AST::PrintStmtNode>(
            std::move(t_Expr)
        );
    }
    
    std::unique_ptr<AST::ExprStmtNode> Parser::parseExprStmt()
    {
        std::unique_ptr<AST::ExpressionNode> t_Expr;
        
        t_Expr = parseExpression();
        require(TokenType::SEMICOLON);
        
        return std::make_unique<AST::ExprStmtNode>(
            std::move(t_Expr)
        );
    }
    
    std::unique_ptr<AST::BlockStmtNode> Parser::parseBlockStmt()
    {
        std::vector<std::unique_ptr<AST::DeclarationNode>> t_decls;
        
        require(TokenType::OPEN_FIGURE);
        while (!matching(TokenType::CLOSE_FIGURE))
        {
            t_decls.push_back(parseDeclaration());
        }
        
        return std::make_unique<AST::BlockStmtNode>(
            std::move(t_decls)
        );
    }
    
    std::unique_ptr<AST::IfStmtNode> Parser::parseIfStmt()
    {
        std::unique_ptr<AST::ExpressionNode> t_Condition;
        std::unique_ptr<AST::StatementNode> t_TrueStmt;
        std::unique_ptr<AST::StatementNode> t_FalseStmt;
        
        require(TokenType::IF);
        require(TokenType::OPEN_PAREN);
        t_Condition = parseExpression();
        require(TokenType::CLOSE_PAREN);
        t_TrueStmt = parseStatement();
        if (matching(TokenType::ELSE))
        {
            t_FalseStmt = parseStatement();
        }
        
        return std::make_unique<AST::IfStmtNode>(
            std::move(t_Condition), 
            std::move(t_TrueStmt), 
            std::move(t_FalseStmt)
        );
    }
    
    std::unique_ptr<AST::ExpressionNode> Parser::parseExpression()
    {
        std::unique_ptr<AST::ExpressionNode> t_Expression = nullptr;
        
        t_Expression = parseExpressionComma();
        
        return std::move(t_Expression);
    }
    
    std::unique_ptr<AST::ExpressionCommaNode> Parser::parseExpressionComma()
    {
        std::unique_ptr<AST::NodeNode> left = parseAssignmentExpr();
        
        while (matching(TokenType::COMMA))
        {
            Token op = previousToken();
            std::unique_ptr<AST::NodeNode> right = parseAssignmentExpr();
            left = std::make_unique<AST::ExpressionCommaNode>(std::move(left), op, std::move(right);
        }
        
        return left;
    }
    
    std::unique_ptr<AST::AssignmentExprNode> Parser::parseAssignmentExpr()
    {
        std::unique_ptr<AST::NodeNode> left = parseTernaryExpr();
        
        if (matching(TokenType::EQUALS))
        {
            Token op = previousToken();
            std::unique_ptr<AST::NodeNode> right = parseAssignmentExpr();
            return std::make_unique<AST::AssignmentExprNode>(std::move(left), op, std::move(right));
        }
        
        return left;
    }
    
    std::unique_ptr<AST::TernaryExprNode> Parser::parseTernaryExpr()
    {
        std::unique_ptr<AST::LogicOrExprNode> t_MainExpr;
        std::unique_ptr<AST::LogicOrExprNode> t_TrueExpr;
        
        t_MainExpr = parseLogicOrExpr();
        require(TokenType::QUESTION);
        t_TrueExpr = parseLogicOrExpr();
        require(TokenType::COLON);
        t_FalseExpr = parseTernaryExpr();
        
        return std::make_unique<AST::TernaryExprNode>(
            std::move(t_MainExpr), 
            std::move(t_TrueExpr), 
            std::move(t_FalseExpr)
        );
    }
    
    std::unique_ptr<AST::LogicOrExprNode> Parser::parseLogicOrExpr()
    {
        std::unique_ptr<AST::NodeNode> left = parseLogicAndExpr();
        
        while (matching(TokenType::OR))
        {
            Token op = previousToken();
            std::unique_ptr<AST::NodeNode> right = parseLogicAndExpr();
            left = std::make_unique<AST::LogicOrExprNode>(std::move(left), op, std::move(right);
        }
        
        return left;
    }
    
    std::unique_ptr<AST::LogicAndExprNode> Parser::parseLogicAndExpr()
    {
        std::unique_ptr<AST::NodeNode> left = parseEqualityExpr();
        
        while (matching(TokenType::AND))
        {
            Token op = previousToken();
            std::unique_ptr<AST::NodeNode> right = parseEqualityExpr();
            left = std::make_unique<AST::LogicAndExprNode>(std::move(left), op, std::move(right);
        }
        
        return left;
    }
    
    std::unique_ptr<AST::EqualityExprNode> Parser::parseEqualityExpr()
    {
        std::unique_ptr<AST::NodeNode> left = parseComparisonExpr();
        
        while (matching(TokenType::EQUALS_EQUALS, TokenType::BANG_EQUALS))
        {
            Token op = previousToken();
            std::unique_ptr<AST::NodeNode> right = parseComparisonExpr();
            left = std::make_unique<AST::EqualityExprNode>(std::move(left), op, std::move(right);
        }
        
        return left;
    }
    
    std::unique_ptr<AST::ComparisonExprNode> Parser::parseComparisonExpr()
    {
        std::unique_ptr<AST::NodeNode> left = parseTermExpr();
        
        while (matching(TokenType::GREATER_EQUALS, TokenType::GREATER, TokenType::LESS, TokenType::LESS_EQUALS))
        {
            Token op = previousToken();
            std::unique_ptr<AST::NodeNode> right = parseTermExpr();
            left = std::make_unique<AST::ComparisonExprNode>(std::move(left), op, std::move(right);
        }
        
        return left;
    }
    
    std::unique_ptr<AST::TermExprNode> Parser::parseTermExpr()
    {
        std::unique_ptr<AST::NodeNode> left = parseFactorExpr();
        
        while (matching(TokenType::PLUS, TokenType::MINUS))
        {
            Token op = previousToken();
            std::unique_ptr<AST::NodeNode> right = parseFactorExpr();
            left = std::make_unique<AST::TermExprNode>(std::move(left), op, std::move(right);
        }
        
        return left;
    }
    
    std::unique_ptr<AST::FactorExprNode> Parser::parseFactorExpr()
    {
        std::unique_ptr<AST::NodeNode> left = parseUnaryExpr();
        
        while (matching(TokenType::ASTERISK, TokenType::DIVISION, TokenType::PERCENT))
        {
            Token op = previousToken();
            std::unique_ptr<AST::NodeNode> right = parseUnaryExpr();
            left = std::make_unique<AST::FactorExprNode>(std::move(left), op, std::move(right);
        }
        
        return left;
    }
    
    std::unique_ptr<AST::UnaryExprNode> Parser::parseUnaryExpr()
    {
        std::unique_ptr<AST::PrimaryExprNode> t_Expr;
        
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
            t_Expr = parseUnaryExpr();
        }
        else
        {
            t_Expr = parsePrimaryExpr();
        }
        
        return std::make_unique<AST::UnaryExprNode>(
            std::move(t_Op), 
            std::move(t_Expr)
        );
    }
    
    std::unique_ptr<AST::PrimaryExprNode> Parser::parsePrimaryExpr()
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
            parseExpression();
            require(TokenType::CLOSE_PAREN);
        }
        
        return std::make_unique<AST::PrimaryExprNode>(
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
    
} // Lox

