#include <stdio.h>
#include <stdarg.h>
#include <stddef.h>
#include <setjmp.h>
#include <cmocka.h>
#include "../src/scanner.h"
#include <unistd.h>
#include <string.h>

static void test_scanner_init(void **state) {
	(void)state;
	char cwd[1024];
	getcwd(cwd, sizeof(cwd));
	char *filename = strcat(cwd,"/test/testFiles/testScannerInit.txt");	
	char *expected = "I am initting the scanner!\n\nThis is the end of the file.\n\0";
	char *result = initScanner(filename);	
	assert_string_equal(result, expected);
}

int main(void) {

	const struct CMUnitTest tests[] = {
		cmocka_unit_test(test_scanner_init), 
	};

	return cmocka_run_group_tests(tests, NULL, NULL);

}
