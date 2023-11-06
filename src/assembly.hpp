
#pragma once

#include "ir.hpp"

#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <cctype>

class AssemblyGenerator
{
public:
    AssemblyGenerator(const IRGenerator &ir) : ir(ir) {}

    std::string generateNASM()
    {
#if defined(_WIN32)
        return genenerateNasmWin32();
#elif defined(__APPLE__)
        return generateNasmApple();
#elif defined(__linux__)
        return generateNasmLinux();
#endif
        std::cerr << "Invalid operating system";
        return "";
    }

private:
    const IRGenerator &ir;

    std::string dataSection()
    {
        std::stringstream ss;
        ss << "section .data\n";
        ss << "format db \"%d\", 0x0a, 0\n"; // "%d\n\0" format string
        ss << "number dq 0\n";               // Use dq for a 64-bit (double quadword) space
        return ss.str();
    }

    std::string bssSection()
    {
        std::stringstream ss;
        ss << "section .bss\n";
        for (const auto &var : ir.getVariables())
        {
            ss << var << " resq 1\n"; // Reserve a quadword (64-bit) for each variable
        }
        return ss.str();
    }

    std::string generateNasmApple()
    {
        std::stringstream ss;

        ss << dataSection();
        ss << bssSection();

        ss << "section .text\n";
        ss << "global _start\n";
        ss << "extern _printf\n";
        ss << "_start:\n";

        for (const auto &node : ir.getIR())
        {
            switch (node.getType())
            {
            case IRNode::PRINT_NUMBER:
                // Assuming PRINT_NUMBER node contains either a number or a variable's name
                // Check if it's a number
                if (node.getNumValue())
                {
                    // Store the number in the 'number' variable and call printf
                    ss << "    mov qword [rel number], " << node.getNumValue() << "\n";
                    ss << "    mov rsi, qword [rel number]\n"; // Pass the number as an argument to printf
                }
                else
                {
                    // If it's not a number, it must be a variable. Print the variable's value
                    ss << "    mov rsi, [rel " << node.getVarName() << "]\n"; // Pass the variable's value as an argument to printf
                }
                ss << "    lea rdi, [rel format]\n"; // Pass the format string as an argument to printf
                ss << "    mov rax, 0\n";            // No vector registers used
                ss << "    call _printf\n";
                break;
            case IRNode::ASSIGN:
                // Move the immediate value into the designated variable
                ss << "    mov qword [rel " << node.getVarName() << "], " << node.getNumValue() << "\n";
                break;
            case IRNode::MOVE_RETURN_VALUE:
                // Move the immediate value into the RAX register
                ss << "    mov rax, 0x2000001\n"; // System call number for sys_exit

                if (node.getNumValue())
                {
                    ss << "    mov rdi, " << node.getNumValue() << "\n"; // Use the literal value as the exit code
                }
                else
                {
                    ss << "    mov rdi, [rel " << node.getVarName() << "]\n"; // Use the literal value as the exit code
                }
                break;
            default:
                std::cerr << "Unsupported node type: " << node.getType() << std::endl;
                break;
            }
        }

        // Properly terminate the program with a system call to exit
        ss << "    syscall\n"; // Call kernel

        return ss.str();
    }
    std::string generateNasmLinux()
    {
        std::stringstream ss;
        ss << dataSection();

        return ss.str();
    }

    std::string generateNasmWin32()
    {
        std::stringstream ss;
        ss << dataSection();

        return ss.str();
    }
};
