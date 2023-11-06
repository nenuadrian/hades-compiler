#pragma once

#include "parser.hpp"

#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <cctype>

class IRNode
{
public:
    enum IRType
    {
        MOVE_RETURN_VALUE, // Represents an instruction to move the return value to the return register
        PRINT_NUMBER,      // Prints to console
        ASSIGN,            // Assigns a value to a variable
    };

    // Constructor for MOVE_RETURN_VALUE and PRINT_NUMBER where only one value is needed
    IRNode(IRType type, int64_t numValue) : type(type), numValue(numValue) {}

    // Constructor for ASSIGN where a variable name and a value is needed
    IRNode(IRType type, const std::string &varName, int64_t numValue = 0) : type(type), varName(varName), numValue(numValue) {}

    IRType getType() const { return type; }
    const std::string &getVarName() const { return varName; }
    int64_t getNumValue() const { return numValue; }

private:
    IRType type;
    std::string varName; // Used for variable names in ASSIGN type
    int64_t numValue;    // Used for numeric values
};

class IRGenerator
{
public:
    explicit IRGenerator(const ASTNode &astNode, std::vector<const std::string> variables) : variables(variables)
    {
        generateIR(astNode);
    }

    const std::vector<IRNode> &getIR() const { return ir; }
    const std::vector<const std::string> &getVariables() const { return variables; }

private:
    std::vector<IRNode> ir;
    std::vector<const std::string> variables;

    void generateIR(const ASTNode &node)
    {
        switch (node.getToken().getType())
        {
        case TokenType::HERO:
        {
            // 'hero' declares a variable and assigns a number to it
            // 'hero x = 5;'
            const auto &varName = node.getChildren()[0].getToken().getValue();
            const auto &numberStr = node.getChildren()[1].getToken().getValue();

            // No need to convert to int here as we are dealing with strings
            ir.emplace_back(IRNode::IRType::ASSIGN, varName, std::stoll(numberStr));
            break;
        }
        case TokenType::STYX:
        {
            // STYX node is for printing the value of a variable, so get the variable name
            const auto &varName = node.getChildren()[0].getToken().getValue();
            ir.emplace_back(IRNode::IRType::PRINT_NUMBER, varName);
            break;
        }
        case TokenType::BESTOW:
        {
            // BESTOW node assigns a return value, which we assume to be a number

            if (node.getChildren()[0].getToken().getType() == TokenType::NUMBER)
            {
                const auto &numberStr = node.getChildren()[0].getToken().getValue();
                ir.emplace_back(IRNode::IRType::MOVE_RETURN_VALUE, std::atoi(numberStr.c_str()));
            }
            else if (node.getChildren()[0].getToken().getType() == TokenType::IDENTIFIER)
            {
                const auto &numberStr = node.getChildren()[0].getToken().getValue();
                ir.emplace_back(IRNode::IRType::MOVE_RETURN_VALUE, numberStr);
            }

            break;
        }

        default:
            // Process other node types if necessary
            break;
        }

        // Recursively generate IR for the children nodes if needed
        for (const auto &child : node.getChildren())
        {
            generateIR(child);
        }
    }
};
