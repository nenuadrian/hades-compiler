#pragma once

#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <cctype>
#include <vector>
#include <unordered_map>

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
        std::string token;
        std::string line;
        while (std::getline(code, line))
        {
            if (line.empty() || line.front() == '#')
                continue;

            for (const char character : line)
            {
                switch (character)
                {
                case ';':
                case '=':
                    addToken(token);
                    addToken(std::string(1, character));
                    token.clear();
                    break;
                default:
                    if (std::isspace(character))
                    {
                        addToken(token);
                        token.clear();
                    }
                    else if (std::isalnum(character))
                    {
                        token += character;
                    }
                    break;
                }
            }
        }
    }

private:
    static const std::unordered_map<std::string, TokenType> tokenMap;

    void addToken(const std::string &s)
    {
        static const std::unordered_map<std::string, TokenType> tokenMap = createTokenMap();

        if (s.empty())
            return;
        auto it = tokenMap.find(s);
        if (it != tokenMap.end())
        {
            tokens.emplace_back(it->second, s);
        }
        else if (is_number(s))
        {
            tokens.emplace_back(TokenType::NUMBER, s);
        }
        else
        {
            tokens.emplace_back(TokenType::IDENTIFIER, s);
        }
    }

    static std::unordered_map<std::string, TokenType> createTokenMap()
    {
        std::unordered_map<std::string, TokenType> map;
        map["hero"] = TokenType::HERO;
        map["bestow"] = TokenType::BESTOW;
        map["styx"] = TokenType::STYX;
        map[";"] = TokenType::SEMICOLON;
        map["="] = TokenType::EQUALS;
        return map;
    }

    bool is_number(const std::string &s)
    {
        return std::all_of(s.begin(), s.end(), ::isdigit);
    }
};