#ifndef _IUGERUM_PARSER_H_
#define _IUGERUM_PARSER_H_

#include "lexer.h"

typedef struct _syntax_tree_node {
	TOKEN_TYPE op;
	struct _syntax_tree_node *first, *second;
	number num;
} syntax_tree_node;

syntax_tree_node* alloc_syntax_tree_node(void);
syntax_tree_node* clone_syntax_tree(syntax_tree_node *tree);
void free_syntax_tree(syntax_tree_node *tree);

syntax_tree_node *parse_input(void);

#endif