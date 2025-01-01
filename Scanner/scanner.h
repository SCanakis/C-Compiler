#ifndef SCANNER_H
#define SCANNER_H


typedef enum {
	TOK_INT, // int
	TOK_RETURN, //  return
	TOK_IF,	// if
	TOK_ELSE, // else
	TOK_IDENTIFIER, // x, myFunction
	TOK_NUMBER, // 123, 2.1
	TOK_ASSIGN, // =
	TOK_EQ, // == 
	TOK_PLUS, // +
	TOK_MINUS, // -
	TOK_STAR, // *
	TOK_SLASH, // / 
	TOK_LPAREN, // (
	TOK_RPAREN, // )
	TOK_LBRACKET, // {
	TOK_RBRACKET, // }
	TOK_SEMICOLON, // ;
	TOK_COMMA, // ,
	TOK_EOF, // end fo file
	TOK_ERROR // unrecognized token
} tokenType;


typedef struct token_s {
	tokenType type;
	char *lexemel;
	int line;
	int col;
	int intValue;
} token_t;



token_t getNextToken();

void initScanner(const char *filename);

void closeScanner();


#endif
