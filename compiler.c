#include <stdio.h>
#include <ctype.h>
#include "lexer.h"
#include "parser.h"
#include "codegen.h"

int main(int argc, char *argv[]) {
	FILE *spec = fopen(SPEC_FILE, "r");
	double a, b;
	fscanf(spec, "%lf%lf", &a, &b);
	set_lexer_input(spec);
	
// 	printf("Compiling first function...\n");
	syntax_tree_node *tree1 = parse_input();
// 	printf("Compiling second function...\n");
	syntax_tree_node *tree2 = parse_input();
// 	printf("Compiling third function...\n");
	syntax_tree_node *tree3 = parse_input();
	
	FILE *out = fopen("code.asm", "w");
	generate_code(tree1, tree2, tree3, a, b, out);
	fclose(out);
	
	fclose(spec);
}
