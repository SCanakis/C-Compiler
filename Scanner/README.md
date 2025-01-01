## SCANNER:

This read me file is more so I can organize my thought in regards to scanner.c and .h 


Requierments Scanner need to follow:

- Remove all whitespace and comments
- Be able to classify the following types of symbols
    - Literals ("strings", 123, 12.2, etc.)
    - Identifiers (x, myFunction)
    - Arithemtic Operations (+, -, \*, /, =, ==)
    - Delimiters or Sperators ((), {}, , , ;)
 
Main functions:

token\_t getNextToken():
    A function to get the next token from input stream or buffer

void initScanner(const char \*filename):
    Make the scanner with a file

void closeScanner():
    Clearn up scanner resource 

