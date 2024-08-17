#ifndef AST_H
#define AST_H

typedef struct AST_NODE AST_NODE;

typedef enum {
    AST_NUMERIC,
    AST_STRING,
    AST_IDENTIFIER,
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
    AST_NODE *left;
    AST_NODE *right;
    char *op;
} BinaryExpr;

typedef struct AST_NODE {
    AST_TYPE type;

    union {
        NumericExpr numeric_expr;
        StringExpr string_expr;
        IdentifierExpr identifier_expr;
        BinaryExpr binary_expr;
    } node;

} AST_NODE;

typedef struct {
    AST_NODE **body;
    size_t count;
} Program;

void print_ast(Program *ast);
void print_ast_node(AST_NODE *node, int indent_level);

#endif