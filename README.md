# Hades Compiler

[![Java CI with Gradle](https://github.com/nenuadrian/hades-compiler/actions/workflows/gradle.yml/badge.svg)](https://github.com/nenuadrian/hades-compiler/actions/workflows/gradle.yml)

![hades](assets/hades.png)

## Table of Contents

- [Hades Compiler](#hades-compiler)
  - [Table of Contents](#table-of-contents)
  - [Overview](#overview)
  - [Purpose](#purpose)
  - [Compiler Design](#compiler-design)
  - [Syntax](#syntax)
  - [Build \& Run](#build--run)
    - [Build](#build)
    - [Run](#run)
    - [Test](#test)
  - [Disclaimer](#disclaimer)

## Overview

Compiler for a custom language, Hades, to JVM byte-code using ANTLR for parsing as an opportunity to explore the inner-depth of compiler prototyping methodology.

The new language is not expected to beat any other in any meaningul way in terms of syntax or performance, but I might focus on bringing different capabilities from different languages experimentally.


I use ANTLR as it is quite commonly known and leveraged by production languages and we can find definition files for them such as:
  * Dart: https://github.com/antlr/grammars-v4/blob/master/dart2/
  * Swift: https://github.com/antlr/grammars-v4/blob/master/swift/swift3/Swift3.g4
  * Countlness others: https://github.com/antlr/grammars-v4/tree/master

## Purpose

The primary goal is to create a basic compiler capable of solving simple LeetCode problems and scripting-level programs. This is an educational passion project to understand compiler construction, not intended for production use or to solve a direct problem with other languages.

## Compiler Design

The compiler will follow a a fairly traditional structure:

1. **Tokenization**: Breaking down the source code into tokens.
2. **Abstract Syntax Tree (AST)**: Converting tokens into an AST.
3. **Code Generation**: Translating the AST into byte-code/assembly/executable code.

## Syntax

Hello world example:

```
hero a = 2; # variable
hero b = 3; # variable
styx a; # prints a
bestow a; # returns a
```

Defined in src/main/antlr/Hades.g4.

## Build & Run

Gradlew based. Use gradlew.bat for Windows.

### Build

```
./gradlew build
```

### Run

```
./gradlew run --args="--help"
```

### Test

```
./gradlew test test
```


## Disclaimer

Provided as is with no maintenance or guarantees.
