#include "lexer.h"

#include <ctype.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>

static FILE* input_file;
static TOKEN_TYPE cur_tok;
static number cur_number;
static bool is_eof;

void set_lexer_input(FILE *in) {
	input_file = in;
}

number current_number(void) {
	return cur_number;
}

static char *next_string(void) {
	int capacity = 10;
	char *str = malloc(capacity);
	int i = 0, ch;
	is_eof = false;
	
	do {
		ch = getchar();
	} while (ch == ' ' || ch == '\t');
	
	do {
		str[i] = ch;
		if (++i == capacity) {
			capacity *= 2;
			str = realloc(str, capacity);
		}
	} while ((ch = getchar()) != EOF && !isspace(ch));
	if (ch == '\n' || ch == '\r' || ch == EOF) is_eof = true;
	str[i] = 0;
	
	return str;
}

static TOKEN_TYPE current_token_internal(void) {
	char *str = next_string();
	
	if (strcmp("cos", str) == 0) {
		free(str);	
		return TOKEN_COS;
	}
	if (strcmp("sin", str) == 0) {
		free(str);
		return TOKEN_SIN;
	}
	if (strcmp("tan", str) == 0) {
		free(str);
		return TOKEN_TAN;
	}
	if (strcmp("ctg", str) == 0) {
		free(str);
		return TOKEN_CTG;
	}
	if (strcmp("e", str) == 0) {
		free(str);
		return TOKEN_E;
	}
	if (strcmp("pi", str) == 0) {
		free(str);
		return TOKEN_PI;
	}
	if (strcmp("x", str) == 0) {
		free(str);
		return TOKEN_X;
	}
	if (strcmp("+", str) == 0) {
		free(str);
		return TOKEN_PLUS;
	}
	if (strcmp("-", str) == 0) {
		free(str);
		return TOKEN_MINUS;
	}
	if (strcmp("*", str) == 0) {
		free(str);
		return TOKEN_MUL;
	}
	if (strcmp("/", str) == 0) {
		free(str);
		return TOKEN_DIV;
	}
	if (is_eof) {
		free(str);
		return TOKEN_EOF;
	}
	sscanf(str, "%lf", &cur_number);
	free(str);
	return TOKEN_NUMBER;
}

void next_token(void) {
	cur_tok = current_token_internal();
}

TOKEN_TYPE current_token(void) {
	return cur_tok;
}
