#ifndef AST_H
#define AST_H

typedef enum {
    AST_NUMERIC,
    AST_STRING,
    AST_BINARY_EXPR
} AST_TYPE;

typedef struct {
    char *value;
} NumericExpr;

typedef struct {
    char *value;
} StringExpr;

typedef struct {
    char *value;
} IdentifierExpr;


typedef struct {
    AST_TYPE type;

    union {
        NumericExpr numeric_expr;
        StringExpr string_expr;
        IdentifierExpr identifier_expr;
    } node;

} AST_NODE;

typedef struct {
    AST_NODE *body;
} Program;

#endif