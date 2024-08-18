#ifndef TRANSPILER_H
#define TRANSPILER_H

#include"ast.h"

typedef struct {

} Transpiler;

void generate(Program *ast);
void generate_stmt(AST_NODE *node, FILE *fptr);
void generate_expression(AST_NODE *node, FILE *output);

#endif