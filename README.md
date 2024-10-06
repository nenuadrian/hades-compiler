# Hades Compiler

![hades](assets/hades.png)

## Overview

Compiler to JVM byte-code using ANTLR for parsing as an opportunity to explore the inner-depth of combile prototyping.

## Purpose

The primary goal is to create a basic compiler capable of solving simple LeetCode problems and scripting-level programs. This is an educational passion project to understand compiler construction, not intended for production use.

## Compiler Structure

The compiler will follow a traditional structure:

1. **Tokenization**: Breaking down the source code into tokens.
2. **Abstract Syntax Tree (AST)**: Converting tokens into an AST.
3. **Code Generation**: Translating the AST into assembly code.

## Language

Hello world example:

```
hero a = 2; # variable
hero b = 3; # variable
styx a; # prints a
bestow a; # returns a
```

## Disclaimer

Provided as is with no maintenance or guarantees.
