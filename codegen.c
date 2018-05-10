#include "codegen.h"

#include <stdlib.h>
#include <stdint.h>
#include "differentiator.h"

static void generate_code_ft_rec(syntax_tree_node *tree,
	FILE *out,
	int stackDepth, int esp);

static void generate_code_ft_rec(syntax_tree_node *tree,
	FILE *out,
	int stackDepth, int esp) {
	// supposed that when you call this functions there are at least 
	// 2 free slots in stack
	if (stackDepth < 2) {
		fprintf(stderr, "Code generation error! Stack is empty!\n");
		abort();
		return;
	}
	switch (tree->op) {
	// zero-operand operations
	case TOKEN_X:
		fprintf(out, "    fld qword[ebp+8]\n");
		return;
	case TOKEN_NUMBER:
	{
		union {
			double d;
			uint64_t u64;
		} value;
		value.d = tree->num;
// 		fprintf(out, "    push qword 0x%llx\n", value.u64); // only in 64-bit mode
		fprintf(out, "    push dword 0x%x\n", (uint32_t)(value.u64 >> 32));
		fprintf(out, "    push dword 0x%x\n", (uint32_t)value.u64);
		fprintf(out, "    fld qword[esp+%u]\n", esp-8);
		fprintf(out, "    add esp, 8\n");
		return;
	}
	case TOKEN_E:
		fprintf(out, "    fldl2e\n");
		fprintf(out, "    f2xm1\n");
		fprintf(out, "    fld1\n");
		fprintf(out, "    faddp\n");
		return;
	case TOKEN_PI:
		fprintf(out, "    fldpi\n");
		return;
	case TOKEN_PLUS:	
	case TOKEN_MINUS:	
	case TOKEN_SIN:	
	case TOKEN_COS:	
	case TOKEN_TAN:	
	case TOKEN_CTG:	
	case TOKEN_DIV:	
	case TOKEN_MUL:	
	case TOKEN_EOF:	
		break;
	}
	// one-operand operations
	generate_code_ft_rec(tree->first, out, stackDepth, esp);
	switch (tree->op) {
	case TOKEN_SIN:
		fprintf(out, "    fsin\n");
		return;
	case TOKEN_COS:
		fprintf(out, "    fcos\n");
		return;
	case TOKEN_TAN:
		fprintf(out, "    fptan\n");
		fprintf(out, "    fdivp\n");
		return;
	case TOKEN_CTG:
		fprintf(out, "    fptan\n");
		fprintf(out, "    fdivrp\n");
		return;
	case TOKEN_PLUS:	
	case TOKEN_MINUS:	
	case TOKEN_X:	
	case TOKEN_NUMBER:	
	case TOKEN_E:	
	case TOKEN_PI:	
	case TOKEN_DIV:	
	case TOKEN_MUL:	
	case TOKEN_EOF:	
		break;
	}
	// two-operand operations
	esp -= 8;
	fprintf(out, "    sub esp, 8\n");
	fprintf(out, "    fstp qword[esp+%u]\n", esp);
	generate_code_ft_rec(tree->second, out, stackDepth, esp);

	switch (tree->op) {
	case TOKEN_PLUS:
		fprintf(out, "    fadd qword[esp+%u]\n", esp);
		break;
	case TOKEN_MINUS:
		fprintf(out, "    fsubr qword[esp+%u]\n", esp);
		break;
	case TOKEN_MUL:
		fprintf(out, "    fmul qword[esp+%u]\n", esp);
		break;
	case TOKEN_DIV:
		fprintf(out, "    fdivr qword[esp+%u]\n", esp);
		break;
	case TOKEN_X:
	case TOKEN_NUMBER:	
	case TOKEN_E:	
	case TOKEN_PI:	
	case TOKEN_SIN:	
	case TOKEN_COS:	
	case TOKEN_TAN:	
	case TOKEN_CTG:		
	case TOKEN_EOF:	
		break;
	}
	
	fprintf(out, "    add esp, 8\n");
	esp += 8;
}

void generate_code_from_tree(syntax_tree_node *tree, 
	FILE *assembly_output) {
	generate_code_ft_rec(tree, assembly_output, 8, 0);
}

static void generate_code_func_prologue(FILE *assembly_output) {
	fprintf(assembly_output, "    push ebp\n");
	fprintf(assembly_output, "    mov ebp, esp\n");
}

static void generate_code_func_epilogue(FILE *assembly_output) {
	fprintf(assembly_output, "    leave\n");
	fprintf(assembly_output, "    ret\n");
}

void generate_code(syntax_tree_node *tree1, 
					syntax_tree_node *tree2, 
					syntax_tree_node *tree3,
					FILE *assembly_output) {
	fprintf(assembly_output, "section .text\n");
	fprintf(assembly_output, "global f1, f2, f3, g1, g2, g3\n");
	fprintf(assembly_output, "f1:\n");
	generate_code_func_prologue(assembly_output);
	generate_code_from_tree(tree1, assembly_output);
	generate_code_func_epilogue(assembly_output);
	fprintf(assembly_output, "f2:\n");
	generate_code_func_prologue(assembly_output);
	generate_code_from_tree(tree2, assembly_output);
	generate_code_func_epilogue(assembly_output);
	fprintf(assembly_output, "f3:\n");
	generate_code_func_prologue(assembly_output);
	generate_code_from_tree(tree3, assembly_output);
	generate_code_func_epilogue(assembly_output);
	fprintf(assembly_output, "g1:\n");
	generate_code_func_prologue(assembly_output);
	generate_code_from_tree(differentiate(tree1), assembly_output);
	generate_code_func_epilogue(assembly_output);	
	fprintf(assembly_output, "g2:\n");
	generate_code_func_prologue(assembly_output);
	generate_code_from_tree(differentiate(tree2), assembly_output);
	generate_code_func_epilogue(assembly_output);	
	fprintf(assembly_output, "g3:\n");
	generate_code_func_prologue(assembly_output);
	generate_code_from_tree(differentiate(tree3), assembly_output);
	generate_code_func_epilogue(assembly_output);	
}