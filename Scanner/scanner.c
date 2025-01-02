#include <stdio.h>
#include "scanner.h"
#include <stdlib.h>
#include <string.h>




static char *source = NULL; // Will hold entire file contents
static size_t pos = 0; // Current index in the source
static int line = 1; // Current line number
static int column = 0; // Current column number
static int length = 0; // Current column number

// Skips to next line updating col, line, and pos
void skipToNextLine() {

	while(source[pos] != '\n') {
		pos++;
	}

	pos++;
	line++;
	col = 0;
}


// Gets next non-whitesapce character
char getNextChar() {
	char currChar = source[pos];
	pos++;
	column++;

	// Checks for whitespace or next line
	while(isspace(currChar) || currChar == '\n || currChar == '/' ) {
		
		// Handles Comments
		if(currChar == '/' && source[pos] == '/') {
			skipToNextLine()
		}
		

		// Handles skipping lines and blank space
		if(currChar == '\n') {
			skipToNextLine();

		} else if(isspace(currChar)) {
			pos++;
			col++;
		}
		currChar = source[pos];
	}

	return currChar;
}


// Skips to next line without affecting col or line
int skipFakeLine(int tempPos) {

	while(source[tempPos] != '\n') {
		tempPos++;
	}

	return tempPos++;
}

char peekNextChar() {
	int tempPos = pos;

	char currChar = source[tempPos];
	tempPos++;

	// Checks for whitespace or next line
	while(isspace(currChar) || currChar == '\n || currChar == '/' ) {
		
		// Handles Comments
		if(currChar == '/' && source[pos] == '/') {
			tempPos = skipFakeLine(tempPos);
		}
		
		// Handles skipping lines and blank space
		if(currChar == '\n') {
			skipToNextLine();

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
		currFloat = atoi(getNextChar());
		totalFloat += currFloat * (10 ** -(decimalCount));
	}
	return totalFloat;

}

// Allocates Memory for Lexeme and copys string.
char* createLexeme(int initalPos) {
	char * lexeme = (char *) malloc(pos-initalPos-1);
	
	if(lexeme == NULL) {
		perror("Could not allocate enough memory for string. Error occured in line: %d , column: %d. \n", line, col);
		exit(EXIT_FAILURE);
	}

	strncpy(lexeme, source + initalPos,(size_t) (pos - initialPos));
	return lexeme;
}

// Sets Line and Column of Token
void setLC(token_t **tokenArg) {
	token_t *token = *tokenArg;
	token->col = col;
	token->line = line;
}


// Helper function to get indentifiers and KeyWords
void identifiersAndKeyWords(token_t **tokenArg) {


}

token_t getNextToken() {
	
	int initalPos = pos;

	// Allocates Memory and Error handling
	token_t *token = (token_t *) malloc(sizeof(token_t));
	
	if(token == NULL) {
		perror("Could not allocated memory for token");
		exit(EXIT_FAILURE);
	}

	char currChar = getNextChar(&currentPos); 


	// Checks whether currChar is a digits or currChar is 
	if(isdigit(currChar)) {

		int currInt = atoi(&currChar);
		int totalInt = 0;

		// Iterates until number ends 
		while(isdigit(peekNextChar()) || peekNextChar() == '.' ) {
			
			// Handles numbers higher than 9
			if(isdigit(peekNextChar())) {
				total = total * 10 +  currInt;
				currInt = atoi(getNextChar());
			} else {
				// Handle Deciamls
				token->floatValue = getFloat(currInt, totalInt);
				token->type = TOK_FLOAT_NUMBER;
				setLC(&token);
				return token;	
			}

		}	
		// Handles Integers
		total = total * 10 + currInt;
		token->intValue = totalInt;
		token->type = TOK_INT_NUMBER;
		setLC(&token);
		return token;

	// This section should chekc for: Identifiers and Keywords 
	} else if (isalpha(currChar) || currChar == '_') {
	
		

	} else {
		checkForSymbols();
		checkForOperators();
	}

	return NULL;
}


void initScanner(const char *filename) {


	FILE *fp = fopen(filename, "r");

	if(fp == NULL) {
		perror("Error opening the file");
		exit(EXIT_FAILURE);		
	}

	
	fseek(fp, 0, SEEK_END);
	length = ftell(fp); // Finds length of fp
	fseek(fp, 0, SEEK_SET);

	source = (char *) malloc(length+1); 

	if(source == NULL) {
		perror("Malloc failed");
		exit(EXIT_FAILURE);
	}

	fread(source, 1, length, fp);

	source[length] = '\0';
	fclose(fp);
}





int main(int argc, char* argv[]) {
	
	
	initScanner(argv[1]);


	for(int i = 0; source[i] != '\0'; i++) {
		printf("%c", source[i]);
	}
	

	free(source);
}
