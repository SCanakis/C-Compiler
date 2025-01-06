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
	free(int_token_real);
	free(int_token);
	printf("[INT TEST PASSED]\n");

	// FLOAT:
	token_t *float_token_real = getNextToken();
	assert_string_equal(float_token->lexeme, float_token_real->lexeme);
	assert_int_equal(float_token->type, float_token_real->type);
	free(float_token_real);
	free(float_token);
	printf("[FLOAT TEST PASSED]\n");

	// CHAR: 
	token_t *char_token_real = getNextToken();
	assert_string_equal(char_token->lexeme, char_token_real->lexeme);
	assert_int_equal(char_token->type, char_token_real->type);
	free(char_token_real);
	free(char_token);
	printf("[CHAR TEST PASSED]\n");
	
	// RETURN: 
	token_t *return_token_real = getNextToken();
	assert_string_equal(return_token->lexeme, return_token_real->lexeme);
	assert_int_equal(return_token->type, return_token_real->type);
	free(return_token_real);
	free(return_token);
	getNextToken();
	printf("[RETURN TEST PASSED]\n");
	
	// IF: 
	token_t *if_token_real = getNextToken();
	assert_string_equal(if_token->lexeme, if_token_real->lexeme);
	assert_int_equal(if_token->type, if_token_real->type);
	free(if_token_real);
	free(if_token);
	getNextToken();
	printf("[IF TEST PASSED]\n");
	
	// ELSE: 
	token_t *else_token_real = getNextToken();
	assert_string_equal(else_token->lexeme, else_token_real->lexeme);
	assert_int_equal(else_token->type, else_token_real->type);
	free(else_token_real);
	free(else_token);
	getNextToken();
	printf("[ELSE TEST PASSED]\n");
	
	// FOR: 
	token_t *for_token_real = getNextToken();
	assert_string_equal(for_token->lexeme, for_token_real->lexeme);
	assert_int_equal(for_token->type, for_token_real->type);
	free(for_token_real);
	free(for_token);
	getNextToken();
	printf("[FOR TEST PASSED]\n");

	// WHILE: 
	token_t *while_token_real = getNextToken();
	assert_string_equal(while_token->lexeme, while_token_real->lexeme);
	assert_int_equal(while_token->type, while_token_real->type);
	free(while_token_real);
	free(while_token);
	getNextToken();
	printf("[WHILE TEST PASSED]\n");

	// EOF:
	token_t *eof = getNextToken();

	assert_int_equal(eof->type, TOK_EOF);
	printf("[EOF TEST PASSED]\n");

	closeScanner();
}

static void test_identifiers(void **state) {

	// SETUP:
	(void)state;

	char cwd[1024];
	getcwd(cwd,sizeof(cwd));
	char *filename = strcat(cwd, "/test/testFiles/testIdentifier.txt");

	printf("%s\n", filename);	
	token_t *identifier_token = make_token(TOK_IDENTIFIER, "thisIsAnIdentifier");

	// INVOKE:
	initScanner(filename);
	
	token_t *identifier_token_real = getNextToken();
	printf("%s\n", identifier_token_real->lexeme);
	// ASSERT: 

	assert_string_equal(identifier_token->lexeme, identifier_token_real->lexeme);
	assert_int_equal(identifier_token->type, identifier_token_real->type);
	token_t *eof = getNextToken();

	assert_int_equal(eof->type, TOK_EOF);
	closeScanner();
}
int main(void) {

	const struct CMUnitTest tests[] = {
		cmocka_unit_test(test_scanner_init), 
		cmocka_unit_test(test_keywords),
		cmocka_unit_test(test_identifiers),
	};

	return cmocka_run_group_tests(tests, NULL, NULL);

}
