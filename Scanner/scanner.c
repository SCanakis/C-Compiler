#include <stdio.h>
#include "scanner.h"
#include <stdlib.h>
#include <string.h>




static char *source = NULL; // Will hold entire file contents
static size_t pos = 0; // Current index in the source
static int line = 1; // Current line number
static int column = 0; // Current column number
static int length = 0; // Current column number


void skipToNextLine() {

	while(source[pos] != '\n') {
		pos++;
	}

	pos++;
	line++;
	col = 0;
}


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

token_t getNextToken() {

	char currChar = getNextChar(); 

	// Checks whether currChar is a digits or currChar is 
	if(isdigit(currChar)) {
		int currInt = atoi(&currChar);
		int totalInt = 0;
		double totalDouble = 0;
		double currDouble = 0;
		while(isdigit(peekNextChar()) || peekNextChar() == '.' ) {
			
			if(isdigit(peekNextChar())) {
				total = total * 10 +  currInt;
				currInt = atoi(getNextChar());
			} else {
				// Handle Deciamls
			}

		}	

	
	} else if (isalpha(currChar)) {

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
