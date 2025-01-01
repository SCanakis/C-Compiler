#include <stdio.h>
#include "scanner.h"
#include <stdlib.h>
#include <string.h>




static char *source = NULL; // Will hold entire file contents
static size_t pos = 0; // Current index in the source
static int line = 1; // Current line number
static int column = 0; // Current column number
static int length = 0; // Current column number




token_t getNextToken() {
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
