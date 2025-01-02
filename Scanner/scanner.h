#ifndef SCANNER_H
#define SCANNER_H


typedef enum {
	TOK_INT, // keyword: int
	TOK_FLOAT, // keyword: floats
	TOK_RETURN, //  keyword: return
	TOK_IF,	// keyword: if
	TOK_ELSE, // keyword: else
	TOK_FOR, // keyword: for
	TOK_WHILE, // keyword: while 
	TOK_IDENTIFIER, // x, myFunction
	TOK_INT_NUMBER, // 123
	TOK_FLOAT_NUMBER, // 1.23
	TOK_ASSIGN, // =
	TOK_EQ, // == 
	TOK_LT, // <
	TOK_GT,	// >
	TOK_LTE, // <=
	TOK_GTE, // >=
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
	char *lexeme;
	int line;
	int col;
	int intValue;
	float floatValue;
} token_t;



token_t *getNextToken();

void initScanner(const char *filename);

void closeScanner();


#endif
