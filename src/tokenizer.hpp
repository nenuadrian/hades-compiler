#pragma once

#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <cctype>

enum TokenType
{
    BESTOW,
    STYX,
    NUMBER,
    SEMICOLON,
    HERO,
    IDENTIFIER,
    EQUALS,
    UNKNOWN
};

class Token
{
public:
    Token(TokenType type, const std::string &value) : type(type), value(value) {}

    TokenType getType() const { return type; }
    const std::string &getValue() const { return value; }

private:
    TokenType type;
    std::string value;
};

class Tokenizer
{

public:
    std::vector<Token> tokens;

    Tokenizer(std::stringstream &code)
    {
        std::stringstream token;
        std::string line;
        while (std::getline(code, line))
        {
            if (line.front() == '#')
            { // Skip comments
                continue;
            }
            else
            {
                for (const char character : line)
                {
                    if (std::isspace(character))
                    {
                        addToken(token.str());
                        token.str(std::string());
                    }
                    else if (std::isalnum(character))
                    {
                        token << character;
                    }
                    else if (character == ';')
                    {
                        addToken(token.str());
                        addToken(";");
                        token.str(std::string());
                    }
                    else if (character == '=')
                    {
                        token << character;
                        addToken(token.str());
                        token.str(std::string());
                    }
                }
            }
        }
    }

private:
    void addToken(const std::string &s)
    {
        if (s == "hero")
        {
            tokens.push_back(Token(TokenType::HERO, s));
        }
        else if (is_number(s))
        {
            tokens.push_back(Token(TokenType::NUMBER, s));
        }
        else if (s == "bestow")
        {
            tokens.push_back(Token(TokenType::BESTOW, s));
        }
        else if (s == "styx")
        {
            tokens.push_back(Token(TokenType::STYX, s));
        }
        else if (s == ";")
        {
            tokens.push_back(Token(TokenType::SEMICOLON, s));
        }
        else if (s == "=")
        {
            tokens.push_back(Token(TokenType::EQUALS, s));
        }
        else if (!s.empty())
        {
            // Assume any other string is an identifier
            tokens.push_back(Token(TokenType::IDENTIFIER, s));
        }
    }

    bool is_number(const std::string &s)
    {
        std::string::const_iterator it = s.begin();
        while (it != s.end() && std::isdigit(*it))
            ++it;
        return !s.empty() && it == s.end();
    }
};
