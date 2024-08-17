#include "token.h"

#ifndef PARSER_H
#define PARSER_H

typedef struct {
    Token *tokens;
    int current;
} Parser;

Parser *init_parser(Token *tokens);
AST_NODE *parse_ast(Parser *parser);

#endif