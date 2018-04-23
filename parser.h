#ifndef _IUGERUM_PARSER_H_
#define _IUGERUM_PARSER_H_

#include "lexer.h"

typedef struct _syntax_tree_node {
	TOKEN_TYPE op;
	struct _syntax_tree_node *first, *second;
	number num;
} syntax_tree_node;

syntax_tree_node *parse_input(void);

#endif