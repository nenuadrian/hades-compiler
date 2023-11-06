
#include "tokenizer.hpp"
#include "parser.hpp"
#include "ir.hpp"
#include "assembly.hpp"

#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <cctype>

int main(int argc, char *argv[])
{
    if (argc < 2)
    {
        std::cerr << "Usage: " << argv[0] << " <filename>\n";
        return 1;
    }

    std::ifstream file(argv[1]);
    if (!file.is_open())
    {
        std::cerr << "Could not open file: " << argv[1] << std::endl;
        return 1;
    }

    std::stringstream buffer;
    buffer << file.rdbuf();

    file.close();

    Tokenizer tokenizer = Tokenizer(buffer);
    Parser parser = Parser(tokenizer.tokens);
    IRGenerator gen = IRGenerator(parser.parse(), parser.getVariables());
    AssemblyGenerator assembly = AssemblyGenerator(gen);

    const std::string nasmCode = assembly.generateNASM();
    std::cout << nasmCode;

    std::ofstream outputFile("output.asm");
    if (outputFile.is_open())
    {
        outputFile << nasmCode;
        outputFile.close();
        std::cout << "NASM code has been written to output.asm\n";
    }
    else
    {
        std::cerr << "Unable to open file for writing.\n";
        return 1; // Return an error code.
    }

    return 0;
}
