#ifndef _IUGERUM_LEXER_H_
#define _IUGERUM_LEXER_H_

#include <stdio.h>

typedef double number;

typedef enum {TOKEN_X, TOKEN_NUMBER, 
	TOKEN_PLUS, TOKEN_MINUS, TOKEN_MUL, TOKEN_DIV,
	TOKEN_E, TOKEN_PI,
	TOKEN_SIN, TOKEN_COS, TOKEN_TAN, TOKEN_CTG,
	TOKEN_EOF} TOKEN_TYPE;

void set_lexer_input(FILE *in);
TOKEN_TYPE current_token(void);
number current_number(void);
void next_token(void);

#endif