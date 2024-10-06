grammar Hades;

// Lexer Rules
HERO: 'hero';           // Keyword to declare a variable
STYX: 'styx';           // Keyword to print a value
BESTOW: 'bestow';       // Keyword to return a value
ID: [a-zA-Z]+;          // Identifier for variables (e.g., 'a', 'variable')
NUMBER: [0-9]+;         // Numeric literals (e.g., '42')
ASSIGN: '=';
SEMICOLON: ';';
WS: [ \t\r\n]+ -> skip; // Whitespace (ignored)

// Parser Rules
program: statement+;    // A program consists of one or more statements

statement: varDeclaration    // Variable declaration
         | printStatement    // Print statement
         | returnStatement;  // Return statement

varDeclaration: HERO ID ASSIGN NUMBER SEMICOLON;    // hero a = 2;
printStatement: STYX ID SEMICOLON;                  // styx a;
returnStatement: BESTOW ID SEMICOLON;               // bestow a;