#include "differentiator.h"
#include <stdlib.h>

syntax_tree_node* differentiate(syntax_tree_node *tree) {
	if (!tree) {
		return NULL;
	}
	if (tree->op == TOKEN_X) {
		tree->op = TOKEN_NUMBER;
		tree->num = 1;
	} else if (tree->op == TOKEN_NUMBER || tree->op == TOKEN_PI 
		|| tree->op == TOKEN_E) {
		tree->op = TOKEN_NUMBER;
		tree->num = 0;
	} else if (tree->op == TOKEN_PLUS || tree->op == TOKEN_MINUS) {
		tree->first = differentiate(tree->first);
		tree->second = differentiate(tree->second);
	} else if (tree->op == TOKEN_MUL) {
		syntax_tree_node *t = clone_syntax_tree(tree), *new_node;
		tree->first = differentiate(tree->first);
		t->second = differentiate(t->second);
		new_node = alloc_syntax_tree_node();
		new_node->op = TOKEN_PLUS;
		new_node->first = tree;
		new_node->second = t;
		tree = new_node;
	} else if (tree->op == TOKEN_DIV) {
		syntax_tree_node *t1 = clone_syntax_tree(tree->second), 
						 *t2 = clone_syntax_tree(tree->second);
		syntax_tree_node *denominator = alloc_syntax_tree_node();
		denominator->op = TOKEN_MUL;
		denominator->first = t1;
		denominator->second = t2;
		
		syntax_tree_node *first_term = alloc_syntax_tree_node();
		first_term->op = TOKEN_MUL;
		first_term->first = differentiate(clone_syntax_tree(tree->first));
		first_term->second = tree->second;
		syntax_tree_node *second_term = alloc_syntax_tree_node();
		second_term->op = TOKEN_MUL;
		second_term->first = differentiate(clone_syntax_tree(tree->second));
		second_term->second = tree->first;
		
		syntax_tree_node *difference = alloc_syntax_tree_node();
		difference->op = TOKEN_MINUS;
		difference->first = first_term;
		difference->second = second_term;
		
		tree->first = difference;
		tree->second = denominator;
	} else if (tree->op == TOKEN_SIN) {
		tree->op = TOKEN_COS;
		syntax_tree_node *product = alloc_syntax_tree_node();
		product->op = TOKEN_MUL;
		product->first = tree;
		product->second = differentiate(clone_syntax_tree(tree->first));
		tree = product;
	} else if (tree->op == TOKEN_COS) {
		tree->op = TOKEN_SIN;
		syntax_tree_node *product = alloc_syntax_tree_node();
		product->op = TOKEN_MUL;
		product->first = tree;
		product->second = differentiate(clone_syntax_tree(tree->first));
		tree = product;
		
		syntax_tree_node *diff = alloc_syntax_tree_node();
		diff->op = TOKEN_MINUS;
		diff->first = alloc_syntax_tree_node();
		diff->first->op = TOKEN_NUMBER;
		diff->first->num = 0;
		diff->second = tree;
		tree = diff;
	} else if (tree->op == TOKEN_TAN) {
		syntax_tree_node *nominator = differentiate(clone_syntax_tree(tree->first));
		syntax_tree_node *denominator = alloc_syntax_tree_node();
		denominator->op = TOKEN_MUL;
		denominator->first = alloc_syntax_tree_node();
		denominator->first->op = TOKEN_COS;
		denominator->first->first = tree->first;
		denominator->second = clone_syntax_tree(denominator->first);
		tree->op = TOKEN_DIV;
		tree->first = nominator;
		tree->second = denominator;
	} else if (tree->op == TOKEN_CTG) {
		syntax_tree_node *nominator = differentiate(clone_syntax_tree(tree->first));
		syntax_tree_node *denominator = alloc_syntax_tree_node();
		denominator->op = TOKEN_MUL;
		denominator->first = alloc_syntax_tree_node();
		denominator->first->op = TOKEN_SIN;
		denominator->first->first = tree->first;
		denominator->second = clone_syntax_tree(denominator->first);
		tree->op = TOKEN_DIV;
		tree->first = nominator;
		tree->second = denominator;
		
		syntax_tree_node *diff = alloc_syntax_tree_node();
		diff->op = TOKEN_MINUS;
		diff->first = alloc_syntax_tree_node();
		diff->first->op = TOKEN_NUMBER;
		diff->first->num = 0;
		diff->second = tree;
		tree = diff;
	} else {
		fprintf(stderr, "Differentiation error: unknown function!");
		abort();
	}
	return tree;
}