package com.example;

import java.io.FileOutputStream;

public class Main {
    public static void main(String[] args) throws Exception {
        // Input Hades code
        String input = "hero a = 2;\nhero b = 3;\nstyx a;";

        // Generate bytecode
        byte[] bytecode = new HadesCompiler().generateBytecode(input);

        // Save the bytecode to a .class file
        try (FileOutputStream fos = new FileOutputStream("HadesProgram.class")) {
            fos.write(bytecode);
        }

        System.out.println("Bytecode has been successfully generated and saved as HadesProgram.class.");
    }
}
