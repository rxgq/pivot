#ifndef TRANSPILER_H
#define TRANSPILER_H

#include"ast.h"

void generate(Program *ast);
void generate_stmt(AST_NODE *node, FILE *fptr);
void generate_expression(AST_NODE *node, FILE *output);
void generate_proc_call(AST_NODE *node, FILE *fptr);

#endif