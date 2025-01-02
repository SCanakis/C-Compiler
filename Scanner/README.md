## SCANNER:

This read-me file is more so I can organize my thoughts in regards to scanner.c and .h 


Requirements Scanner needs to follow:

- Remove all whitespace and comments
- Be able to classify the following types of symbols
    - Literals ("strings", 123, 12.2, etc.)
    - Identifiers (x, myFunction)
    - Arithmetic Operations (+, -, \*, /, =, ==, <, >, <=. >=)
    - Delimiters or Sperators ((), {}, ",", ;, .)
 
Main functions:

token\_t getNextToken():
    A function to get the next token from the input stream or buffer

void initScanner(const char \*filename):
    Make the scanner with a file

void closeScanner():
    Clean up scanner resource 

