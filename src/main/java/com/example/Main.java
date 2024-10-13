package com.example;

import java.io.File;
import java.io.FileOutputStream;
import java.io.IOException;
import java.nio.charset.StandardCharsets;
import java.nio.file.Files;

import org.apache.commons.cli.CommandLine;
import org.apache.commons.cli.CommandLineParser;
import org.apache.commons.cli.DefaultParser;
import org.apache.commons.cli.HelpFormatter;
import org.apache.commons.cli.Options;
import org.apache.commons.cli.ParseException;

public class Main {

    public static void main(String[] args) throws Exception {
        Options options = new Options();
        options.addOption("h", "help", false, "Show help");
        options.addRequiredOption("i", "input", true, "Input .hades file or directory");

        CommandLineParser parser = new DefaultParser();
        HelpFormatter formatter = new HelpFormatter();

        try {
            // Parse the command-line arguments
            CommandLine cmd = parser.parse(options, args);

            // If the user asks for help, display it and exit
            if (cmd.hasOption("h")) {
                formatter.printHelp("HadesCompiler", options);
                System.exit(0);
            }

            execute(cmd);
        } catch (ParseException e) {
            System.out.println(e.getMessage());
            formatter.printHelp("HadesCompiler", options);
            System.exit(1);
        }
    }

    private static String readFileContents(File file) throws IOException {
        // Read all lines from the file and convert them to a string
        String content = new String(Files.readAllBytes(file.toPath()), StandardCharsets.UTF_8);
        return content;
    }

    private static void execute(CommandLine cmd) throws Exception {
        File file = new File(cmd.getOptionValue("f"));
        compile(file);
    }

    private static void compile(File file) throws Exception {
        if (file.isDirectory()) {
            File[] files = file.listFiles();
            if (files != null) {  // Check if the directory is not empty or accessible
                for (File childFile : files) {
                    if (childFile.isFile()) {
                        compile(file);
                    }
                }
            }
        } else {

            // Check if the file has a .hades extension
            if (!file.getName().endsWith(".hades")) {
                System.out.println("Error: The file must have a .hades extension.");
                System.exit(1);
            }

            // Remove the .hades extension and replace it with .class
            String classFilePath = file.getAbsolutePath().substring(0, file.getAbsolutePath().length() - 6) + ".class";

            // Read the contents of the .hades file
            String input = readFileContents(file);

            // Generate bytecode (assuming the HadesCompiler class exists)
            byte[] bytecode = new HadesCompiler().generateBytecode(input);

            // Write the bytecode to a .class file
            try (FileOutputStream fos = new FileOutputStream(classFilePath)) {
                fos.write(bytecode);
                System.out.println("Class file created: " + classFilePath);
            } catch (IOException e) {
                System.out.println("Error writing to class file: " + e.getMessage());
            }
        }
    }
}
