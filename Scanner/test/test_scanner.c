#include <stdio.h>
#include <stdarg.h>
#include <stddef.h>
#include <setjmp.h>
#include <cmocka.h>
#include "../src/scanner.h"
#include <unistd.h>
#include <string.h>
#include <stdlib.h>


static void test_scanner_init(void **state) {
	(void)state;
	char cwd[1024];
	getcwd(cwd, sizeof(cwd));
	char *filename = strcat(cwd,"/test/testFiles/testScannerInit.txt");	
	char *expected = "I am initting the scanner!\n\nThis is the end of the file.\n\0";
	char *result = initScanner(filename);	
	assert_string_equal(result, expected);
	closeScanner();
}


void free_token(token_t * token) {
	//printf("freeing %s\n ", token->lexeme);
	free(token->lexeme);
	free(token);

}

token_t *make_token(tokenType type, char *lexeme) {
	token_t *token = (token_t*) malloc(sizeof(token_t));
	if(token == NULL) {
		perror("Could not allocate memory for test tokens.\n");
		exit(EXIT_FAILURE);
	}
	token->type = type;
	token->lexeme = lexeme;
	return token;
}


static void test_keywords(void **state) {
	// SETUP:
	(void) state;
	char cwd[1024];
	getcwd(cwd,sizeof(cwd));
	char *filename = strcat(cwd, "/test/testFiles/testKeywords.txt");

	token_t *int_token = make_token(TOK_INT, "int");
	token_t *float_token = make_token(TOK_FLOAT, "float");
	token_t *char_token = make_token(TOK_CHAR, "char");
	token_t *return_token = make_token(TOK_RETURN, "return");
	token_t *if_token = make_token(TOK_IF, "if");
	token_t *else_token = make_token(TOK_ELSE, "else");
	token_t *for_token = make_token(TOK_FOR, "for");
	token_t *while_token = make_token(TOK_WHILE, "while");
	
	initScanner(filename);
	
	// INT:
	token_t *int_token_real = getNextToken();
	assert_string_equal(int_token->lexeme, int_token_real->lexeme);
	assert_int_equal(int_token->type, int_token_real->type);
	free_token(int_token_real);
	free(int_token);
	printf("[INT TEST PASSED]\n");

	// FLOAT:
	token_t *float_token_real = getNextToken();
	assert_string_equal(float_token->lexeme, float_token_real->lexeme);
	assert_int_equal(float_token->type, float_token_real->type);
	free_token(float_token_real);
	free(float_token);
	printf("[FLOAT TEST PASSED]\n");

	// CHAR: 
	token_t *char_token_real = getNextToken();
	assert_string_equal(char_token->lexeme, char_token_real->lexeme);
	assert_int_equal(char_token->type, char_token_real->type);
	free_token(char_token_real);
	free(char_token);
	printf("[CHAR TEST PASSED]\n");
	
	// RETURN: 
	token_t *return_token_real = getNextToken();
	assert_string_equal(return_token->lexeme, return_token_real->lexeme);
	assert_int_equal(return_token->type, return_token_real->type);
	free_token(return_token_real);
	free(return_token);
	getNextToken();
	printf("[RETURN TEST PASSED]\n");
	
	// IF: 
	token_t *if_token_real = getNextToken();
	assert_string_equal(if_token->lexeme, if_token_real->lexeme);
	assert_int_equal(if_token->type, if_token_real->type);
	free_token(if_token_real);
	free(if_token);
	getNextToken();
	printf("[IF TEST PASSED]\n");
	
	// ELSE: 
	token_t *else_token_real = getNextToken();
	assert_string_equal(else_token->lexeme, else_token_real->lexeme);
	assert_int_equal(else_token->type, else_token_real->type);
	free_token(else_token_real);
	free(else_token);
	getNextToken();
	printf("[ELSE TEST PASSED]\n");
	
	// FOR: 
	token_t *for_token_real = getNextToken();
	assert_string_equal(for_token->lexeme, for_token_real->lexeme);
	assert_int_equal(for_token->type, for_token_real->type);
	free_token(for_token_real);
	free(for_token);
	getNextToken();
	printf("[FOR TEST PASSED]\n");

	// WHILE: 
	token_t *while_token_real = getNextToken();
	assert_string_equal(while_token->lexeme, while_token_real->lexeme);
	assert_int_equal(while_token->type, while_token_real->type);
	free_token(while_token_real);
	free(while_token);
	getNextToken();
	printf("[WHILE TEST PASSED]\n");

	// EOF:
	token_t *eof = getNextToken();

	assert_int_equal(eof->type, TOK_EOF);
	printf("[EOF TEST PASSED]\n");
	free(eof);
	closeScanner();
}

static void test_identifiers(void **state) {

	// SETUP:
	(void)state;

	char cwd[1024];
	getcwd(cwd,sizeof(cwd));
	char *filename = strcat(cwd, "/test/testFiles/testIdentifier.txt");

	//printf("%s\n", filename);	
	token_t *identifier_token = make_token(TOK_IDENTIFIER, "thisIsAnIdentifier");
	token_t *identifier_token_hello = make_token(TOK_IDENTIFIER, "hello");
	// INVOKE:
	initScanner(filename);
	
	token_t *identifier_token_real = getNextToken();
	token_t *identifier_token_hello_real = getNextToken();

	// ASSERT: 
	assert_string_equal(identifier_token->lexeme, identifier_token_real->lexeme);
	assert_int_equal(identifier_token->type, identifier_token_real->type);	

	free(identifier_token);
	free_token(identifier_token_real);

	assert_string_equal(identifier_token_hello->lexeme, identifier_token_hello_real->lexeme);
	assert_int_equal(identifier_token_hello->type, identifier_token_hello_real->type);
	
	free(identifier_token_hello);
	free_token(identifier_token_hello_real);

	token_t *eof = getNextToken();

	assert_int_equal(eof->type, TOK_EOF);
	closeScanner();

	free(eof);

}

static void test_symbols(void **state) {
	// SETUP:

	(void) state;
	char cwd[1024];
	getcwd(cwd,sizeof(cwd));

	token_t *assign_t = make_token(TOK_ASSIGN, "=");
	token_t *lt_t = make_token(TOK_LT, "<");
	token_t *gt_t = make_token(TOK_GT, ">");
	token_t *plus_t = make_token(TOK_PLUS, "+");
	token_t *minus_t = make_token(TOK_MINUS, "-");
	token_t *star_t = make_token(TOK_STAR, "*");
	token_t *slash_t = make_token(TOK_SLASH, "/");
	token_t *lparen_t = make_token(TOK_LPAREN, "(");
	token_t *rparen_t = make_token(TOK_RPAREN, ")");
	token_t *lbrace_t = make_token(TOK_LBRACE, "{");
	token_t *rbrace_t = make_token(TOK_RBRACE, "}");
	token_t *lbracket_t = make_token(TOK_LBRACKET, "[");
	token_t *rbracket_t = make_token(TOK_RBRACKET, "]");	
	token_t *semicolon_t = make_token(TOK_SEMICOLON, ";");
	token_t *comma_t = make_token(TOK_COMMA, ",");

	char *filename = strcat(cwd, "/test/testFiles/testSymbols.txt");
	initScanner(filename);

	// INVOKE:
	token_t *current_real = getNextToken();	
	assert_string_equal(assign_t->lexeme, current_real->lexeme);
	assert_int_equal(assign_t -> type, current_real->type);
	free(assign_t);
	free_token(current_real);
	printf("[\"=\" TEST PASSES]\n");

	current_real = getNextToken();	
	assert_string_equal(lt_t->lexeme, current_real->lexeme);
	assert_int_equal(lt_t -> type, current_real->type);
	free(lt_t);
	free_token(current_real);
	printf("[\"<\" TEST PASSES]\n");

	current_real = getNextToken();	
	assert_string_equal(gt_t->lexeme, current_real->lexeme);
	assert_int_equal(gt_t -> type, current_real->type);
	free(gt_t);
	free_token(current_real);
	printf("[\">\" TEST PASSES]\n");

	current_real = getNextToken();	
	assert_string_equal(plus_t->lexeme, current_real->lexeme);
	assert_int_equal(plus_t -> type, current_real->type);
	free(plus_t);
	free_token(current_real);
	printf("[\"+\" TEST PASSES]\n");

	current_real = getNextToken();	
	assert_string_equal(minus_t->lexeme, current_real->lexeme);
	assert_int_equal(minus_t -> type, current_real->type);
	free(minus_t);
	free_token(current_real);
	printf("[\"-\" TEST PASSES]\n");
	
	current_real = getNextToken();	
	assert_string_equal(star_t->lexeme, current_real->lexeme);
	assert_int_equal(star_t -> type, current_real->type);
	free(star_t);
	free_token(current_real);
	printf("[\"*\" TEST PASSES]\n");
	
	current_real = getNextToken();	
	assert_string_equal(slash_t->lexeme, current_real->lexeme);
	assert_int_equal(slash_t -> type, current_real->type);
	free(slash_t);
	free_token(current_real);
	printf("[\"/\" TEST PASSES]\n");
	
	current_real = getNextToken();	
	assert_string_equal(rbrace_t->lexeme, current_real->lexeme);
	assert_int_equal(rbrace_t -> type, current_real->type);
	free(rbrace_t);
	free_token(current_real);
	printf("[\"}\" TEST PASSES]\n");

	current_real = getNextToken();	
	assert_string_equal(lbrace_t->lexeme, current_real->lexeme);
	assert_int_equal(lbrace_t -> type, current_real->type);
	free(lbrace_t);
	free_token(current_real);
	printf("[\"{\" TEST PASSES]\n");
	

	current_real = getNextToken();
	assert_string_equal(lbracket_t->lexeme, current_real->lexeme);
	assert_int_equal(lbracket_t->type, current_real->type);
	free(lbracket_t);
	free_token(current_real);
	printf("[\"[\" TEST PASSES]\n");

	current_real = getNextToken();
	assert_string_equal(rbracket_t->lexeme, current_real->lexeme);
	assert_int_equal(rbracket_t->type, current_real->type);
	free(rbracket_t);
	free_token(current_real);
	printf("[\"]\" TEST PASSES]\n");

	current_real = getNextToken();	
	assert_string_equal(rparen_t->lexeme, current_real->lexeme);
	assert_int_equal(rparen_t -> type, current_real->type);
	free(rparen_t);
	free_token(current_real);
	printf("[\")\" TEST PASSES]\n");

	current_real = getNextToken();	
	assert_string_equal(lparen_t->lexeme, current_real->lexeme);
	assert_int_equal(lparen_t -> type, current_real->type);
	free(lparen_t);
	free_token(current_real);
	printf("[\"(\" TEST PASSES]\n");

	current_real = getNextToken();	
	assert_string_equal(comma_t->lexeme, current_real->lexeme);
	assert_int_equal(comma_t -> type, current_real->type);
	free(comma_t);
	free_token(current_real);
	printf("[\",\" TEST PASSES]\n");

	current_real = getNextToken();	
	assert_string_equal(semicolon_t->lexeme, current_real->lexeme);
	assert_int_equal(semicolon_t -> type, current_real->type);
	free(semicolon_t);
	free_token(current_real);
	printf("[\";\" TEST PASSES]\n");
	
	token_t *eof = getNextToken();

	assert_int_equal(eof->type, TOK_EOF);
	closeScanner();

	free(eof);
}



int main(void) {

	const struct CMUnitTest tests[] = {
		cmocka_unit_test(test_scanner_init), 
		cmocka_unit_test(test_keywords),
		cmocka_unit_test(test_identifiers),
		cmocka_unit_test(test_symbols),
	};

	return cmocka_run_group_tests(tests, NULL, NULL);

}
