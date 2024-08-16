#include "token.h"

#ifndef PARSER_H
#define PARSER_H

typedef struct {
    Token *tokens;
    int current;
} Parser;

Parser *init_parser(Token *tokens);
void parse_ast();

#endif