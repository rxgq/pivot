#include <stdlib.h>
#include <stdio.h>
#include "ast.h"

void parser_advance(PARSER *parser) {
    parser->curr++;
}

TOKEN current_token(PARSER *parser) {
    return parser->tokens[parser->curr];
}

void parse_ast(PARSER *parser)
{
    while (parser->tokens[parser->curr].type != EOFT) {
        TOKEN token = current_token(parser);
        printf("Token type: %d, Token value: %s\n", token.type, token.value);
        
        if (token.value != NULL) {
            printf("%s", token.value);
        } else {
            printf("NULL token value encountered\n");
        }

        parser_advance(parser);
    }
}

PARSER *create_parser(TOKEN *tokens)
{
    PARSER *parser = (PARSER *)malloc(sizeof(PARSER));
    parser->curr = 0;
    parser->tokens = tokens;

    return parser;
}
