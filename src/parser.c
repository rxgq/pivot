#include <stdlib.h>
#include <stdio.h>
#include "parser.h"
#include "token.h"
#include "ast.h"

Parser *init_parser(Token *tokens) {
    Parser *parser = (Parser *)malloc(sizeof(Parser));
    parser->tokens = tokens;
    parser->current = 0;
}

AST_NODE *init_node(AST_TYPE type, void *ast_node) {
    AST_NODE *node = (AST_NODE *)malloc(sizeof(AST_NODE));
    if (!node) {
        perror("Failed to allocate memory for AST_NODE");
        exit(EXIT_FAILURE);
    }
    node->type = type;

    switch (type) {
        case AST_NUMERIC: 
            node->node.numeric_expr = *(NumericExpr *)ast_node;
            break;
            
        default:
            free(node);
            fprintf(stderr, "Unknown AST_TYPE\n");
            return NULL;
    }

    return node;
}

AST_NODE *parse_primary(Parser *parser) {
    Token curr = parser->tokens[parser->current];
    
    if (curr.type == NUMBER) {
        NumericExpr expr;
        expr.value = curr.lexeme;
        return init_node(AST_NUMERIC, &expr);
    }
}

AST_NODE *parse_ast(Parser *parser) {
    AST_NODE *last;

    while (parser->tokens[parser->current].type != EOF_TOKEN) {
        AST_NODE *node = parse_primary(parser);
        if (node) {
            last = node;
        }

        parser->current++;
    }

    return last;
}
