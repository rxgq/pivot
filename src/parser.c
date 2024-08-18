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

int expect_as(Parser *parser, TokenType type) {
    Token curr = parser->tokens[parser->current];
    parser_advance(parser);

    if (curr.type != type) {
        printf("Expected %s, got %s", print_token(type), print_token(curr.type));
        exit(EXIT_FAILURE);
    }

    return 0;
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
        
        case AST_BOOL:
            node->node.bool_expr = *(BoolExpr *)ast_node;
            break;

        case AST_IDENTIFIER:
            node->node.identifier_expr = *(IdentifierExpr *)ast_node;
            break;

        case AST_BINARY_EXPR:
            node->node.binary_expr = *(BinaryExpr *)ast_node;
            break;

        case AST_VAR_DEC:
            node->node.var_dec_expr = *(VarDecExpr *)ast_node;
            break;

        case AST_IF_STMT:
            node->node.if_stmt_expr = *(IfStmtExpr *)ast_node;
            break;

        case AST_LOGICAL_EXPR:
            node->node.logical_expr = *(LogicalExpr *)ast_node;
            break;

        case AST_ASSIGNMENT_EXPR:
            node->node.assignment_expr = *(AssignmentExpr *)ast_node;
            break;

        case AST_ECHO_EXPR:
            node->node.echo_expr = *(EchoExpr *)ast_node;
            break;

        case AST_BOOL_DEC:
            node->node.bool_dec_expr = *(BoolDecExpr *)ast_node;
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
    
    if (strcmp(curr.lexeme, "(") == 0) {
        parser_advance(parser);
        AST_NODE *expr = parse_logical_or(parser);
        
        if (!match(parser, ")")) {
            fprintf(stderr, "Expected ')' after expression\n");
            exit(EXIT_FAILURE);
        }
        parser_advance(parser);
        return expr;
    }

    parser_advance(parser);
    if (curr.type == NUMERIC) {
        NumericExpr expr = *(NumericExpr *)malloc(sizeof(NumericExpr));
        expr.value = strdup(curr.lexeme);
        return init_node(AST_NUMERIC, &expr);
    } 
    else if (curr.type == STRING) {
        StringExpr expr = *(StringExpr *)malloc(sizeof(StringExpr));;
        expr.value = strdup(curr.lexeme);
        return init_node(AST_STRING, &expr);
    } 
    else if (curr.type == FALSE_TOK || curr.type == TRUE_TOK) {
        BoolExpr expr = *(BoolExpr *)malloc(sizeof(BoolExpr));
        expr.value = strdup(curr.lexeme);
        return init_node(AST_BOOL, &expr);
    }
    else if (curr.type == BOOL) {
        BoolDecExpr expr = *(BoolDecExpr *)malloc(sizeof(BoolDecExpr));
        expr.value = strdup(curr.lexeme);
        return init_node(AST_BOOL_DEC, &expr);
    }
    else if (curr.type == IDENTIFIER) {
        IdentifierExpr expr= *(IdentifierExpr *)malloc(sizeof(IdentifierExpr));
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

AST_NODE *parse_logical_and(Parser *parser) {
    AST_NODE *left = parse_multiplicative(parser);

    while (match(parser, "and")) {
        Token curr = parser->tokens[parser->current];
        char *op = strdup(curr.lexeme);
        parser_advance(parser);

        AST_NODE *right = parse_primary(parser);

        LogicalExpr *expr = (LogicalExpr *)malloc(sizeof(LogicalExpr));
        expr->left = left;
        expr->op = op;
        expr->right = right;

        left = init_node(AST_LOGICAL_EXPR, expr);
    }

    return left;
}

AST_NODE *parse_logical_or(Parser *parser) {
    AST_NODE *left = parse_logical_and(parser);

    while (match(parser, "or")) {
        Token curr = parser->tokens[parser->current];
        char *op = strdup(curr.lexeme);
        parser_advance(parser);

        AST_NODE *right = parse_logical_and(parser);

        LogicalExpr *expr = (LogicalExpr *)malloc(sizeof(LogicalExpr));
        expr->left = left;
        expr->op = op;
        expr->right = right;

        left = init_node(AST_LOGICAL_EXPR, expr);
    }

    return left;
}

AST_NODE *parse_assignment(Parser *parser) {
    AST_NODE *left = parse_primary(parser);

    if (match(parser, "=")) {
        parser_advance(parser);

        AST_NODE *val = parse_expr(parser);

        expect_as(parser, SEMICOLON);

        AssignmentExpr *expr = (AssignmentExpr *)malloc(sizeof(AssignmentExpr));
        expr->identifier = left->node.identifier_expr.value;
        expr->val = val;

        return init_node(AST_ASSIGNMENT_EXPR, expr);
    }

    return left;
}


AST_NODE *parse_expr(Parser *parser) {
    return parse_assignment(parser);
}

AST_NODE *parse_var_dec(Parser *parser) {
    parser_advance(parser);

    Token identifier = parser->tokens[parser->current];
    parser_advance(parser);

    expect_as(parser, COLON);

    Token type = parser->tokens[parser->current];
    parser_advance(parser);

    expect_as(parser, ASSIGNMENT);

    AST_NODE *val = parse_expr(parser);
    expect_as(parser, SEMICOLON);

    VarDecExpr *expr = (VarDecExpr *)malloc(sizeof(VarDecExpr));
    expr->val = val;
    expr->identifier = identifier.lexeme;
    expr->type = type;

    return init_node(AST_VAR_DEC, expr);
}

AST_NODE *parse_if_stmt(Parser *parser) {
    parser_advance(parser);

    AST_NODE *condition = parse_logical_or(parser);

    expect_as(parser, LBRACE);

    size_t capacity = 1;
    size_t count = 0;
    AST_NODE **consequent = (AST_NODE **)malloc(capacity * sizeof(AST_NODE *));
    
    while (!match(parser, "}")) {
        if (count >= capacity) {
            capacity *= 2;
            consequent = (AST_NODE **)realloc(consequent, capacity * sizeof(AST_NODE *));
        }

        AST_NODE *stmt = parse_stmt(parser);
        if (stmt) {
            consequent[count++] = stmt;
        } else {
            fprintf(stderr, "Unexpected token in if statement body: %s\n", parser->tokens[parser->current].lexeme);
            parser_advance(parser);
        }
    }
    expect_as(parser, RBRACE);

    IfStmtExpr *expr = (IfStmtExpr *)malloc(sizeof(IfStmtExpr));
    expr->condition = condition;
    expr->consequent = consequent;
    expr->body_count = count;

    return init_node(AST_IF_STMT, expr);
}

AST_NODE *parse_echo(Parser *parser) {
    expect_as(parser, ECHO);

    AST_NODE *expr = parse_expr(parser);

    EchoExpr *echo_expr = (EchoExpr *)malloc(sizeof(EchoExpr));
    echo_expr->expr = expr;

    expect_as(parser, SEMICOLON);

    return init_node(AST_ECHO_EXPR, echo_expr);
}

AST_NODE *parse_stmt(Parser *parser) {
    TokenType curr = parser->tokens[parser->current].type;

    switch (curr) {
        case LET:
            return parse_var_dec(parser);
        
        case IF:
            return parse_if_stmt(parser);

        case ECHO:
            return parse_echo(parser);

        case IDENTIFIER:
        case NUMERIC:
        case STRING:
            return parse_expr(parser);

        default:
            fprintf(stderr, "Unexpected stmt token: %s\n", parser->tokens[parser->current].lexeme);
            exit(EXIT_FAILURE);
    }
}

Program *parse_ast(Parser *parser) {
    size_t capacity = 10;
    size_t count = 0;
    AST_NODE **body = (AST_NODE **)malloc(capacity * sizeof(AST_NODE *));

    while (parser->tokens[parser->current].type != EOF_TOKEN) {
        if (count >= capacity) {
            capacity *= 2;
            body = (AST_NODE **)realloc(body, capacity * sizeof(AST_NODE *));
        }

        AST_NODE *node = parse_stmt(parser);
        if (node) {
            body[count++] = node;
        } else {
            fprintf(stderr, "Unexpected token parsing ast: %s\n", parser->tokens[parser->current].lexeme);
            parser_advance(parser);
        }
    }

    Program *program = (Program *)malloc(sizeof(Program));
    program->body = body;
    program->count = count;

    return program;
}