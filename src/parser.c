#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include "parser.h"
#include "token.h"
#include "ast.h"

void parser_advance(Parser *parser) {
    parser->current++;
}

int match(Parser *parser, const char *expected) {
    Token curr = parser->tokens[parser->current];
    return strcmp(curr.lexeme, expected) == 0;
}

Parser *init_parser(Token *tokens) {
    Parser *parser = (Parser *)malloc(sizeof(Parser));
    parser->tokens = tokens;
    parser->current = 0;
    return parser;
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

        case AST_STRING:
            node->node.string_expr = *(StringExpr *)ast_node;
            break;

        case AST_IDENTIFIER:
            node->node.identifier_expr = *(IdentifierExpr *)ast_node;
            break;

        case AST_BINARY_EXPR:
            node->node.binary_expr = *(BinaryExpr *)ast_node;
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
    parser_advance(parser);

    if (curr.type == NUMERIC) {
        NumericExpr expr;
        expr.value = strdup(curr.lexeme);
        return init_node(AST_NUMERIC, &expr);
    } else if (curr.type == STRING) {
        StringExpr expr;
        expr.value = strdup(curr.lexeme);
        return init_node(AST_STRING, &expr);
    } else if (curr.type == IDENTIFIER) {
        IdentifierExpr expr;
        expr.value = strdup(curr.lexeme);
        return init_node(AST_IDENTIFIER, &expr);
    } else {
        return NULL;
    }
}

AST_NODE *parse_multiplicative(Parser *parser) {
    AST_NODE *left = parse_primary(parser);

    while (match(parser, "*") || match(parser, "/") || match(parser, "%")) {
        Token curr = parser->tokens[parser->current];
        char *op = strdup(curr.lexeme);
        parser_advance(parser);

        AST_NODE *right = parse_primary(parser);

        BinaryExpr *expr = (BinaryExpr *)malloc(sizeof(BinaryExpr));
        expr->left = left;
        expr->op = op;
        expr->right = right;

        left = init_node(AST_BINARY_EXPR, expr);
    }

    return left;
}

AST_NODE *parse_additive(Parser *parser) {
    AST_NODE *left = parse_multiplicative(parser);

    while (match(parser, "+") || match(parser, "-")) {
        Token curr = parser->tokens[parser->current];
        char *op = strdup(curr.lexeme);
        parser_advance(parser);

        AST_NODE *right = parse_multiplicative(parser);

        BinaryExpr *expr = (BinaryExpr *)malloc(sizeof(BinaryExpr));
        expr->left = left;
        expr->op = op;
        expr->right = right;

        left = init_node(AST_BINARY_EXPR, expr);
    }

    return left;
}

Program *parse_ast(Parser *parser) {
    size_t capacity = 10;
    size_t count = 0;
    AST_NODE **body = (AST_NODE **)malloc(capacity * sizeof(AST_NODE *));
    if (!body) {
        perror("Failed to allocate memory for Program body");
        exit(EXIT_FAILURE);
    }

    while (parser->tokens[parser->current].type != EOF_TOKEN) {
        if (count >= capacity) {
            capacity *= 2;
            body = (AST_NODE **)realloc(body, capacity * sizeof(AST_NODE *));
            if (!body) {
                perror("Failed to reallocate memory for Program body");
                exit(EXIT_FAILURE);
            }
        }

        AST_NODE *node = parse_additive(parser);

        if (node) {
            body[count++] = node;
        }
    }

    Program *program = (Program *)malloc(sizeof(Program));
    if (!program) {
        perror("Failed to allocate memory for Program");
        exit(EXIT_FAILURE);
    }
    program->body = body;
    program->count = count;

    return program;
}