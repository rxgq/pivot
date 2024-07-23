#include "token.h"

#ifndef AST_H
#define AST_H

typedef struct {
    int curr;
    TOKEN *tokens;
} PARSER;

void parse_ast(PARSER *parser);
PARSER *create_parser(TOKEN *tokens);

#endif