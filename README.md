# Hades Compiler

[![Java CI with Gradle](https://github.com/nenuadrian/hades-compiler/actions/workflows/gradle.yml/badge.svg)](https://github.com/nenuadrian/hades-compiler/actions/workflows/gradle.yml)

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

## Hades Syntax

Hello world example:

```
hero a = 2; # variable
hero b = 3; # variable
styx a; # prints a
bestow a; # returns a
```

Defined in src/main/antlr/Hades.g4.

Similarly to how other production languages are defined such as:
  * Dart: https://github.com/antlr/grammars-v4/blob/master/dart2/
  * Swift: https://github.com/antlr/grammars-v4/blob/master/swift/swift3/Swift3.g4
  * Countlness others: https://github.com/antlr/grammars-v4/tree/master


## Build & Run

Gradle based.

### Build

```
gradle build
```


### Run

```
gradle run
```

## Disclaimer

Provided as is with no maintenance or guarantees.
