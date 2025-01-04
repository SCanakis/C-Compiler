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
	
	}

}

// Gets next non-whitesapce character
char getNextChar() {
	char currChar = source[pos];
	pos++;
	column++;

	// Checks for whitespace or next line
	while(isspace(currChar) || currChar == '\n' || currChar == '/' ) {
		if(currChar == EOF || currChar == -1) {
			return EOF;
		}
		//printf("skipping smth\n");		
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
	//printf("getting next char: %c \t pos = %d\n", currChar, (int)pos -1);
	return currChar;
}


// Skips to next line without affecting col or line
int skipFakeLine(int tempPos) {
	//printf("skipping line fakei\n");
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

	// Checks for whitespace or next line
	while(isspace(currChar) || currChar == '\n' || currChar == '/' ) {
		
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
	// printf("getting float\n");
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
	//printf("creating lexeme\n");
	size_t length = pos - initalPos;
	char * lexeme = (char *) malloc((int)length+1);
	
	if(lexeme == NULL) {
		perror("Could not allocate enough memory for string.");
		exit(EXIT_FAILURE);
	}
	strncpy(lexeme, source + initalPos, length);
	lexeme[length] = '\0';
	return lexeme;
}

// Sets Line and Column of Token
token_t *createToken(tokenType type, int initialPos) {
	//printf("creating token\n");
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



int  enumMap(tokenType type) {

	switch(type) {
		// Should be a space in between this and another character
		case TOK_INT: return 4;
		case TOK_FLOAT: return 6;
		case TOK_CHAR: return 5;
		
		// Not neccesarlly
		case TOK_RETURN: return 6;
		case TOK_IF: return 2;
		case TOK_ELSE: return 4;
		case TOK_FOR: return 3;
		case TOK_WHILE: return 5;
		default: return 0;
	}

}




tokenType checkForKeyWords() {
	//printf("checking for keywords\n");
	char *positionPtr = source + pos - 1;
	//printf("%s\n", positionPtr);
	//printf("%d\n", strncmp(positionPtr, "int ",4) == 0);
	int length;
	if(strncmp(positionPtr, "int ",4) == 0) {
		return TOK_INT;
	}
	if(strncmp(positionPtr, "char ", 5) == 0) {
		return TOK_CHAR;
	}	
	if(strncmp(positionPtr, "float ", 6) == 0) {
		return TOK_FLOAT;
	}
	if(strncmp(positionPtr, "return", 6) == 0) {
		length = enumMap(TOK_RETURN);
		if (isalpha(source[pos+length-1])) {
			return (tokenType) NULL;
		}	
		return TOK_RETURN;
	}
	if(strncmp(positionPtr, "if", 2) == 0) {
		length = enumMap(TOK_IF);
		if (isalpha(source[pos+length-1])) {
			return (tokenType) NULL;
		}	
		return TOK_IF;
	}
	if(strncmp(positionPtr, "else", 4) == 0) {
		length = enumMap(TOK_ELSE);
		if (isalpha(source[pos+length-1])) {
			return (tokenType) NULL;
		}	
		return TOK_ELSE;
	}
	if(strncmp(positionPtr, "for", 3) == 0) {
		length = enumMap(TOK_FOR);
		if (isalpha(source[pos+length-1])) {
			return (tokenType) NULL;
		}
		return TOK_FOR;
	}
	if(strncmp(positionPtr, "while", 5) == 0) {
		length = enumMap(TOK_WHILE);
		if (isalpha(source[pos+length-1])) {
			return (tokenType) NULL;
		}	
		return TOK_WHILE;
	} else {
		return TOK_ERROR;
	}
}



tokenType checkForSymbols() {

	//printf("checking for symbols\n");
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

tokenType checkForOperators() {

	//printf("checking for operators\n");
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





void skipKeyWordPos(int length) {
	for(int i = 0; i < length - 1; i++ ) {
		pos++;
	}
}

// Helper function to get indentifiers and KeyWords
token_t *identifiersAndKeyWords(int initialPos) {
	//printf("doing keywords and identifiers\n");
	tokenType type = checkForKeyWords(); 
	if(type != TOK_ERROR) {

		int skipAmount = enumMap(type);
		skipKeyWordPos(skipAmount);
		return createToken(type, initialPos);	
	} else {
		while(isalpha(peekNextChar()) || peekNextChar() == '_') {
			getNextChar();
		}
		//printf("creating identifier\n");
		return createToken(TOK_IDENTIFIER, initialPos); 
	}
}




token_t *getNextToken() {
	//printf("begining search for next token\n");
	// Sets initial POS and gets that char

	char currChar = getNextChar(); 
	int initialPos = pos-1;
	//printf("%d\n", currChar);
	if(currChar == 0) {
		return createToken(TOK_EOF, initialPos);	
	}
	//printf("checking if digit\n");
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
		tokenType type = checkForSymbols();
		if(type != TOK_ERROR) {
			return createToken(type, initialPos);
		}
		type = checkForOperators();
		if(type != TOK_ERROR) {
			return createToken(type, initialPos);
		}

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


	//for(int i = 0; source[i] != '\0'; i++) {
	//	printf("%c", source[i]);
	//}
	
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
