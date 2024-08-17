#include "token.h"
#include "ast.h"

#ifndef PARSER_H
#define PARSER_H

typedef struct {
    Token *tokens;
    int current;
} Parser;

Parser *init_parser(Token *tokens);
Program *parse_ast(Parser *parser);

#endif