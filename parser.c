#include "parser.h"

#include <stdio.h>
#include <stdlib.h>

static int op_stack_capacity = 0;
static int op_stack_sz = 0;
static syntax_tree_node **op_stack;

int op_stack_size(void) {
	return op_stack_sz;
}

void op_stack_push(syntax_tree_node *node) {
	if (op_stack_sz >= op_stack_capacity) {
		op_stack_capacity *= 2;
		op_stack = realloc(op_stack, 
			op_stack_capacity * sizeof(*op_stack));
	}
	op_stack[op_stack_sz++] = node;
}

syntax_tree_node* op_stack_pop(void) {
	if (op_stack_sz == 0) {
		fprintf(stderr, "Operations stack is empty!");
		abort();
	} else {
		op_stack_sz--;
		return op_stack[op_stack_sz];
	}
}

void op_stack_clear(void) {
	for (int i = 0; i < op_stack_sz; i++) {
		free(op_stack[i]);
	}
	op_stack_sz = 0;
}

syntax_tree_node *parse_input(void) {
	next_token();
	while (current_token() != TOKEN_EOF) {
		syntax_tree_node *new_node = malloc(
							sizeof(syntax_tree_node));
		new_node->first = new_node->second = NULL;
		new_node->op = current_token();
		new_node->num = current_number();
		switch (current_token()) {
		case TOKEN_SIN:
		case TOKEN_COS:
		case TOKEN_TAN:
		case TOKEN_CTG:
			new_node->first = op_stack_pop();
			break;
		case TOKEN_PLUS:
		case TOKEN_MINUS:
		case TOKEN_MUL:
		case TOKEN_DIV:
			new_node->first = op_stack_pop();
			new_node->second = op_stack_pop();
			break;
		case TOKEN_X:
		case TOKEN_E:
		case TOKEN_PI:
		case TOKEN_NUMBER:
		case TOKEN_EOF:
			break;
		}
		op_stack_push(new_node);
		
		next_token();
	}
	
	syntax_tree_node *res = op_stack_pop();
	op_stack_clear();
	return res;
}