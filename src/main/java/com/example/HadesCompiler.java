package com.example;

import java.io.FileOutputStream;

import org.antlr.v4.runtime.*;
import org.antlr.v4.runtime.tree.*;

public class HadesCompiler {
    public static void main(String[] args) throws Exception {
        // Input Hades code
        String input = "hero a = 2;\nhero b = 3;\nstyx a;";

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

        // Generate bytecode
        byte[] bytecode = bytecodeGenerator.generateBytecode();

        // Save the bytecode to a .class file
        try (FileOutputStream fos = new FileOutputStream("HadesProgram.class")) {
            fos.write(bytecode);
        }

        System.out.println("Bytecode has been successfully generated and saved as HadesProgram.class.");
    }
}
