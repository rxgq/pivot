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
AST_NODE *parse_additive(Parser *parser);
AST_NODE *parse_stmt(Parser *parser);
AST_NODE *parse_expr(Parser *parser);

#endif