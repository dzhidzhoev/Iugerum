#ifndef _IUGERUM_CODEGEN_H_
#define _IUGERUM_CODEGEN_H_

#include "parser.h"

void generate_code_from_tree(syntax_tree_node *tree, FILE *assembly_output);

#endif