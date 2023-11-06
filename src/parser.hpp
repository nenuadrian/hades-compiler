
#pragma once

#include "tokenizer.hpp"

#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <cctype>

class ASTNode
{
    Token token;
    std::vector<ASTNode> children;

public:
    explicit ASTNode(const Token &token) : token(token) {}

    void addChild(const ASTNode &node)
    {
        children.push_back(node);
    }

    const Token &getToken() const { return token; }
    const std::vector<ASTNode> &getChildren() const { return children; }
};

class Parser
{
public:
    explicit Parser(const std::vector<Token> &tokens) : tokens(tokens), currentIndex(0) {}

    ASTNode parse()
    {
        if (tokens.empty())
        {
            throw std::runtime_error("No tokens to parse");
        }

        ASTNode rootNode(Token(TokenType::NUMBER, "root")); // Root node of AST

        while (currentIndex < tokens.size())
        {
            Token currentToken = getNextToken();

            if (currentToken.getType() == TokenType::STYX || currentToken.getType() == TokenType::BESTOW)
            {
                ASTNode statementNode = parseStatement(currentToken);
                rootNode.addChild(statementNode);
            }
            else if (currentToken.getType() == TokenType::HERO)
            {
                ASTNode statementNode = parseStatement(currentToken);
                rootNode.addChild(statementNode);
            }
            else
            {
                throw std::runtime_error("Syntax Error: Expected 'styx' or 'bestow'");
            }
        }

        return rootNode;
    }

    const std::vector<const std::string> getVariables() const { return variables; }

private:
    const std::vector<Token> &tokens;
    std::vector<const std::string> variables;
    size_t currentIndex;

    Token getNextToken()
    {
        if (currentIndex >= tokens.size())
        {
            throw std::runtime_error("Syntax Error: Unexpected end of input");
        }
        return tokens[currentIndex++];
    }

    ASTNode parseStatement(const Token &startToken)
    {
        ASTNode statementNode(startToken); // This node will represent the statement

        Token currentToken = getNextToken();

        switch (startToken.getType())
        {
        case TokenType::HERO:
        {
            // Expecting a variable name after "hero"
            if (currentToken.getType() != TokenType::IDENTIFIER)
            {
                throw std::runtime_error("Syntax Error: Expected variable name after 'hero'");
            }
            // Add the variable name as a child of the statement
            statementNode.addChild(ASTNode(currentToken));
            variables.push_back(currentToken.getValue());

            // Now expecting an equals sign
            currentToken = getNextToken();
            if (currentToken.getType() != TokenType::EQUALS)
            {
                throw std::runtime_error("Syntax Error: Expected '=' after variable name");
            }

            // Expecting a number after "="
            currentToken = getNextToken();
            if (currentToken.getType() != TokenType::NUMBER)
            {
                throw std::runtime_error("Syntax Error: Expected a number after '='");
            }
            // Add the number as a child of the statement
            statementNode.addChild(ASTNode(currentToken));

            // Finally, expect a semicolon
            currentToken = getNextToken();
            if (currentToken.getType() != TokenType::SEMICOLON)
            {
                throw std::runtime_error("Syntax Error: Expected ';' after number");
            }

            break;
        }
        case TokenType::BESTOW:
        {
            // Rest of the BESTOW handling code...
            // Expecting a variable or a number after "bestow"
            if (currentToken.getType() != TokenType::IDENTIFIER && currentToken.getType() != TokenType::NUMBER)
            {
                throw std::runtime_error("Syntax Error: Expected variable or number after 'bestow'");
            }
            statementNode.addChild(ASTNode(currentToken));

            // Expecting a semicolon after variable/number
            currentToken = getNextToken();
            if (currentToken.getType() != TokenType::SEMICOLON)
            {
                throw std::runtime_error("Syntax Error: Expected ';' after 'bestow' statement");
            }
            break;
        }
        case TokenType::STYX:
        {
            // Rest of the STYX handling code...
            // Expecting a variable or a number after "styx"
            if (currentToken.getType() != TokenType::IDENTIFIER && currentToken.getType() != TokenType::NUMBER)
            {
                throw std::runtime_error("Syntax Error: Expected variable or number after 'styx'");
            }
            statementNode.addChild(ASTNode(currentToken));

            // Expecting a semicolon after variable/number
            currentToken = getNextToken();
            if (currentToken.getType() != TokenType::SEMICOLON)
            {
                throw std::runtime_error("Syntax Error: Expected ';' after 'styx' statement");
            }
            break;
        }
        // Handle other cases if necessary
        default:
            throw std::runtime_error("Syntax Error: Unknown statement type");
        }

        return statementNode;
    }
};
