package com.example;

import static org.junit.Assert.assertEquals;
import org.junit.Test; // Importing JUnit's assert functions

public class HadesCompilerTest {

    @Test
    public void testCompile() throws Exception {
        // Arrange: create an instance of HadesCompiler and provide some example source code

        String input = "hero a = 2;\nhero b = 3;\nstyx a;";

        // Generate bytecode
        byte[] bytecode = new HadesCompiler().generateBytecode(input);

        assertEquals(315, bytecode.length);
    }
}
