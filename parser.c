#include "parser.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

syntax_tree_node* alloc_syntax_tree_node(void) {
	syntax_tree_node *t = calloc(1, sizeof(*t));
	t->op = TOKEN_EOF;
	return t;
}

syntax_tree_node* clone_syntax_tree(syntax_tree_node *tree) {
	if (!tree) return NULL;
	syntax_tree_node *n = malloc(sizeof(*tree));
	memcpy(n, tree, sizeof(*n));
	n->first = clone_syntax_tree(tree->first);
	n->second = clone_syntax_tree(tree->second);
	return n;
}

void free_syntax_tree(syntax_tree_node *tree) {
	if (!tree) return;
	free_syntax_tree(tree->first);
	free_syntax_tree(tree->second);
	free(tree);
}

static int op_stack_capacity = 0;
static int op_stack_sz = 0;
static syntax_tree_node **op_stack;

int op_stack_size(void) {
	return op_stack_sz;
}

static void debug_op_stack_print() {
	printf("STACK DUMP\n");
	for (int i = 0; i < op_stack_sz; i++) {
		printf("%p\n", op_stack[i]);
	}
	printf("END STACK DUMP\n");
}

void op_stack_push(syntax_tree_node *node) {
	if (op_stack_sz >= op_stack_capacity) {
		if (op_stack_capacity == 0) {
			op_stack_capacity = 1;
		}
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
		free_syntax_tree(op_stack[i]);
	}
	op_stack_sz = 0;
}

syntax_tree_node *parse_input(void) {
	next_token();
	if (current_token() == TOKEN_EOF) next_token();
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