# Hades Compiler

![hades](assets/hades.png)

## Overview
This repository is part of an educational series exploring the complexities of building a compiler for a custom programming language targeting the x86 CPU using NASM (Netwide Assembler). The project is named after Hades, the Greek god of the underworld, blending mythology with technology.

 * [Part 1 - Building a Mythological Programming Language Compiler For an x86 CPU (NASM) — Part I — Hades](https://adriannenu.medium.com/building-a-mythological-programming-language-compiler-for-an-x86-cpu-nasm-part-i-hades-8543c2b6a920)

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

## MacOS

```
./run_mac.sh
```

## Linux

NYI

## Windows

NYI

## Disclaimer

Provided as is with no maintenance or guarantees.
