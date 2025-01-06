#include <stdio.h>
#include "scanner.h"
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include <ctype.h>



static char *source = NULL; // Will hold entire file contents
static size_t pos = 0; // Current index in the source
static int line = 1; // Current line number
static int column = 0; // Current column number
static int length = 0; // Current column number

// Skips to next line updating col, line, and pos
char skipToNextLine() {
	//printf("skipping line\n");
	while(source[pos] != '\n') {
		pos++;
	}

	pos++;
	line++;
	column = 0;
	return source[pos];
}

// Converts tokenType to Strings
char * printType(tokenType type) {
	switch(type) {

		case TOK_INT: return "TOK_INT";
		case TOK_FLOAT: return "TOK_FLOAT";
		case TOK_CHAR: return "TOK_CHAR";	
		case TOK_RETURN: return "TOK_RETURN";
		case TOK_IF: return "TOK_IF";
		case TOK_ELSE: return "TOK_ELSE";
		case TOK_FOR: return "TOK_FOR";
		case TOK_WHILE: return "TOK_WHILE";
		case TOK_IDENTIFIER: return "TOK_IDENTIFIER";
		case TOK_INT_NUMBER: return "TOK_INT_NUMBER";
		case TOK_FLOAT_NUMBER: return "TOK_INT_FLOAT";
		case TOK_ASSIGN: return "TOK_ASSIGN";
		case TOK_LT: return "TOK_LT";
		case TOK_GT: return "TOK_GT";
		case TOK_PLUS: return "TOK_PLUS";
		case TOK_MINUS: return "TOK_MINUS";
		case TOK_STAR: return "TOK_STAR";
		case TOK_SLASH: return "TOK_SLASH";
		case TOK_LPAREN: return "TOK_LPAREN";
		case TOK_RPAREN: return "TOK_RPAREN";
		case TOK_LBRACKET: return "TOK_LBRACKET";
		case TOK_RBRACKET: return "TOK_RBRACKET";
		case TOK_SEMICOLON: return "TOK_SEMICOLON";
		case TOK_COMMA: return "TOK_COMMA";
		case TOK_EOF: return "TOK_EOF";
		case TOK_ERROR: return "TOK_ERROR";
		default: return "TOK_ERROR";	
	}

}

// Gets next non-whitesapce character
char getNextChar() {
	char currChar = source[pos];
	pos++;
	column++;

	// Checks for whitespace, next line or comments
	while(isspace(currChar) || currChar == '\n' || currChar == '/' ) {
		// Handles EOF
		if(currChar == EOF || currChar == -1) {
			return EOF;
		}
		
		// Handles Comments
		if(currChar == '/' && source[pos] == '/') {
			currChar = skipToNextLine();
		}
		
		// Handles skipping lines and blank space
		if(currChar == '\n') {
			currChar = source[pos];
			pos++;
		} else if(isspace(currChar)) {
			currChar = source[pos];
			pos++;
			column++;
		}
	}
	
	return currChar;
}


// Skips to next line without affecting col or line
int skipFakeLine(int tempPos) {
	while(source[tempPos] != '\n') {
		tempPos++;
	}

	return ++tempPos;
}

char peekNextChar() {
	//printf("peeking next char\n");
	int tempPos = pos;

	char currChar = source[tempPos];
	tempPos++;

	// Checks for whitespace, next line, or comments 
	while(isspace(currChar) || currChar == '\n' || currChar == '/' ) {
		// Handles EOF	
		if(currChar == EOF || currChar == -1) {
			return EOF;
		}
		
		// Handles Comments
		if(currChar == '/' && source[pos] == '/') {
			tempPos = skipFakeLine(tempPos);
		}
		
		// Handles skipping lines and blank space
		if(currChar == '\n') {
			tempPos = skipFakeLine(tempPos);

		} else if(isspace(currChar)) {
			tempPos++;
		}

		currChar = source[tempPos];

	}
	return currChar;

}


// Gets the float based on string
float getFloat(int currInt, int totalInt) {
	// Casting int into floats
	float totalFloat = (float)totalInt + (float) currInt;
	float currFloat= 0;
	int decimalCount = 0;
	
	getNextChar();


	while(isdigit(peekNextChar())) {
		decimalCount++;
		currFloat = (((float)getNextChar()) - '0');
		totalFloat += currFloat * pow(10, -(decimalCount));
	}
	return totalFloat;

}

// Allocates Memory for Lexeme and copys string.
char* createLexeme(int initalPos) {
	
	// Length is used to allocted memory for lexeme + error handeling
	size_t length = pos - initalPos;
	char * lexeme = (char *) malloc((int)length+1);
	
	if(lexeme == NULL) {
		perror("Could not allocate enough memory for string.");
		exit(EXIT_FAILURE);
	}

	// Copy string from source to lexeme
	strncpy(lexeme, source + initalPos, length);
	lexeme[length] = '\0';
	return lexeme;
}


// Sets Line and Column of Token
token_t *createToken(tokenType type, int initialPos) {
	
	// Allocates Memory for token + error handeling
	token_t *token = (token_t*) malloc(sizeof(token_t));
	if(token == NULL) {
		perror("Could not allocated memory for token");
		exit(EXIT_FAILURE);
	}
	token->intValue = 0;
       	token->floatValue = 0;	
	if(type != TOK_EOF) {
		token->lexeme = createLexeme(initialPos);
	} else {
		token->lexeme = "EOF";
	}
	token->type = type;
	token->col = column;
	token->line = line;
	return token;
}


// Map for keywords and their lengths
int keywordMap(tokenType type) {

	switch(type) {
		// Should be a space in between this and another character
		case TOK_INT: return 3;
		case TOK_FLOAT: return 5;
		case TOK_CHAR: return 4;
		
		// Not neccesarlly
		case TOK_RETURN: return 6;
		case TOK_IF: return 2;
		case TOK_ELSE: return 4;
		case TOK_FOR: return 3;
		case TOK_WHILE: return 5;
		default: return 0;
	}

}



// Checks whether current pos pointer contains keywords 
tokenType checkForKeyWords() {
	
	char *positionPtr = source + pos - 1;
	int length;
	// Require a space between the keyword and another character
	if(strncmp(positionPtr, "int ",4) == 0 || strncmp(positionPtr, "int\n",4) == 0) {
		return TOK_INT;
	}
	if(strncmp(positionPtr, "char ", 5) == 0 || strncmp(positionPtr, "char\n", 5) == 0) {
		return TOK_CHAR;
	}	
	if(strncmp(positionPtr, "float ", 6) == 0 || strncmp(positionPtr, "float\n",6) == 0) {
		return TOK_FLOAT;
	}


	// Keyword can have a non-alpha character next to it
	// Uses keywordMap to get length
	// Checks if pos+length-1 is alphabetical if so returns null
	
	if(strncmp(positionPtr, "return", 6) == 0) {
		length = keywordMap(TOK_RETURN);
		if (isalpha(source[pos+length-1])) {
			return TOK_ERROR;
		}	
		return TOK_RETURN;
	}
	if(strncmp(positionPtr, "if", 2) == 0) {
		length = keywordMap(TOK_IF);
		if (isalpha(source[pos+length-1])) {
			return TOK_ERROR;
		}	
		return TOK_IF;
	}
	if(strncmp(positionPtr, "else", 4) == 0) {
		length = keywordMap(TOK_ELSE);
		if (isalpha(source[pos+length-1])) {
			return TOK_ERROR;
		}	
		return TOK_ELSE;
	}
	if(strncmp(positionPtr, "for", 3) == 0) {
		length = keywordMap(TOK_FOR);
		if (isalpha(source[pos+length-1])) {
			return TOK_ERROR;
		}
		return TOK_FOR;
	}
	if(strncmp(positionPtr, "while", 5) == 0) {
		length = keywordMap(TOK_WHILE);
		if (isalpha(source[pos+length-1])) {
			return TOK_ERROR;
		}	
		return TOK_WHILE;
	} else {
		return TOK_ERROR;
	}
}


// Checks for symbols such as: () {} ; ,
tokenType checkForSymbols() {

	char *positionPtr = source + pos - 1;
	if(strncmp(positionPtr, "(",1) == 0) {
		return TOK_LPAREN;
	}
	if(strncmp(positionPtr, ")",1) == 0) {
		return TOK_RPAREN;
	}
	if(strncmp(positionPtr, "{",1) == 0) {
		return TOK_LBRACKET;
	}
	if(strncmp(positionPtr, "}",1) == 0) {
		return TOK_RBRACKET;
	}
	if(strncmp(positionPtr, ",",1) == 0) {
		return TOK_COMMA;
	}
	if(strncmp(positionPtr, ";",1) == 0) {
		return TOK_SEMICOLON;
	} else {
		return TOK_ERROR;
	}
	
}

// Checks for arithmetic operators or comparators
tokenType checkForOperators() {

	char *positionPtr = source + pos - 1;
	if(strncmp(positionPtr, "+",1) == 0) {
		return TOK_PLUS;
	}
	if(strncmp(positionPtr, "-",1) == 0) {
		return TOK_MINUS;
	}
	if(strncmp(positionPtr, "*",1) == 0) {
		return TOK_STAR;
	}
	if(strncmp(positionPtr, "/",1) == 0) {
		return TOK_SLASH;
	}
	if(strncmp(positionPtr, "<",1) == 0) {
		return TOK_LT;
	}
	if(strncmp(positionPtr, ">",1) == 0) {
		return TOK_GT;
	}
        
	if(strncmp(positionPtr, "=",1) == 0) {
		return TOK_ASSIGN;
	} else {
		return TOK_ERROR;
	}

}




// If a keyword is identified pos must skip the rest of the characters that compose that keyword to be able to get the next one. 
void skipKeyWordPos(int length) {
	for(int i = 0; i < length - 1; i++ ) {
		pos++;
		column++;
	}
}


// Helper function to get indentifiers and KeyWords
token_t *identifiersAndKeyWords(int initialPos) {
	
	// Checks for keywords
	tokenType type = checkForKeyWords(); 
	if(type != TOK_ERROR) {
		// If keyword skipWord in source and make token
		int skipAmount = keywordMap(type);
		skipKeyWordPos(skipAmount);
		return createToken(type, initialPos);	
	} else {
		while(isalpha(peekNextChar()) || peekNextChar() == '_') {
			getNextChar();
		}
		// Else makes an identifier
		return createToken(TOK_IDENTIFIER, initialPos); 
	}
}


// Gets the next Token in the file. 

token_t *getNextToken() {
	
	// Sets initial POS and gets the char
	char currChar = getNextChar(); 
	int initialPos = pos-1;
	
	// Checks for EOF
	if(currChar == 0) {
		return createToken(TOK_EOF, initialPos);	
	}
	
	// Checks whether currChar is a digits or currChar is 
	if(isdigit(currChar)) {

		// Casts to Digit
		int totalInt = (int)currChar-'0' ;
		int currInt;
		// Iterates until number ends 
		while(isdigit(peekNextChar()) || peekNextChar() == '.' ) {
			
			// Handles numbers higher than 9
			if(isdigit(peekNextChar())) {
				currInt = ((int)getNextChar()) -'0'; 
				//printf("currInt = %d\n",currInt);
				//printf("Total Int = %d\n", totalInt);
				totalInt = (totalInt * 10) +  currInt;
				//printf("Total Int = %d\n\n", totalInt);
			} else {
				// Handle Deciamls
				token_t *token = createToken(TOK_FLOAT_NUMBER, initialPos);
				token->floatValue = getFloat(currInt, totalInt);
				return token;	
			}

		}	
		// Handles Integers
		token_t *token = createToken(TOK_INT_NUMBER, initialPos);
		token->intValue = totalInt;
		return token;

	// This section should check for: Identifiers and Keywords 
	} else if (isalpha(currChar) || currChar == '_') {

		return identifiersAndKeyWords(initialPos);
		
	} 

	else {
		// Checks for Symbols and returns symbol if not TOK_ERROR
		tokenType type = checkForSymbols();
		if(type != TOK_ERROR) {
			return createToken(type, initialPos);
		}
		type = checkForOperators();
		
		// Checks for Operators and returns symbol if not TOK_ERROR
		if(type != TOK_ERROR) {
			return createToken(type, initialPos);
		}

	}
	// Returns TOK_ERROR if all else fails 
	return createToken(TOK_ERROR, initialPos);
}


void closeScanaer() {
	free(source);
}

// Initiates Global variables
char* initScanner(const char *filename) {

	// Open's file and checks and error handeling
	FILE *fp = fopen(filename, "r");

	if(fp == NULL) {
		perror("Error opening the file");
		exit(EXIT_FAILURE);		
	}

	// Finds length of fp
	fseek(fp, 0, SEEK_END);
	length = ftell(fp);
	fseek(fp, 0, SEEK_SET);

	// Allocates memory to store contents of fp and error handeling
	source = (char *) malloc(length+1); 

	if(source == NULL) {
		perror("Malloc failed");
		exit(EXIT_FAILURE);
	}

	// Copies contents of fp into source
	fread(source, 1, length, fp);
	source[length] = '\0';
	
	// Close file
	fclose(fp);
	return source;
}



/*
int main(int argc, char* argv[]) {
	
	
	initScanner(argv[1]);
	
	token_t *token = getNextToken();

	while(token->type != TOK_EOF) {
		char* type = printType(token->type);
		printf("%s : %s, int : %d , float : %f , l : %d , c : %d \n", type, token->lexeme, token->intValue, token->floatValue, token->line, token -> col);	
		token = getNextToken();
	}
	char* type = printType(token->type);
	printf("%s : %s, int : %d , float : %f , l : %d , c : %d \n", type, token->lexeme, token->intValue, token->floatValue, token->line, token -> col);	

	free(source);
}

*/

