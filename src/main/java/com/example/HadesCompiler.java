package com.example;

import org.antlr.v4.runtime.CharStream;
import org.antlr.v4.runtime.CharStreams;
import org.antlr.v4.runtime.CommonTokenStream;
import org.antlr.v4.runtime.tree.ParseTree;

public class HadesCompiler {

    public byte[] generateBytecode(String input) throws Exception {
        // Create a CharStream that reads from the input string
        CharStream charStream = CharStreams.fromString(input);

        // Create a lexer that feeds off of the char stream
        HadesLexer lexer = new HadesLexer(charStream);

        // Create a token stream from the lexer
        CommonTokenStream tokens = new CommonTokenStream(lexer);

        // Create a parser that feeds off of the token stream
        HadesParser parser = new HadesParser(tokens);

        // Begin parsing at the `program` rule
        ParseTree tree = parser.program();

        // Print out the parse tree for debugging purposes
        System.out.println(tree.toStringTree(parser));

        // Visit the parse tree to generate bytecode
        HadesBytecodeGenerator bytecodeGenerator = new HadesBytecodeGenerator();
        bytecodeGenerator.visit(tree);

        return bytecodeGenerator.generateBytecode();
    }
}
