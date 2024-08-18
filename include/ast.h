#ifndef AST_H
#define AST_H

#include "token.h"

typedef struct AST_NODE AST_NODE;

typedef enum {
    AST_NUMERIC,
    AST_STRING,
    AST_BOOL,
    AST_IDENTIFIER,
    AST_BINARY_EXPR,
    AST_VAR_DEC,
    AST_IF_STMT,
    AST_LOGICAL_EXPR,
} AST_TYPE;

typedef struct {
    char *value;
} NumericExpr;

typedef struct {
    char *value;
} StringExpr;

typedef struct {
    char *value;
} BoolExpr;

typedef struct {
    char *value;
} IdentifierExpr;

typedef struct {
    AST_NODE *left;
    AST_NODE *right;
    char *op;
} BinaryExpr;

typedef struct {
    Token type;
    char *identifier;
    AST_NODE *val;
} VarDecExpr;

typedef struct {
    AST_NODE **body;
} BlockExpr;

typedef struct {
    AST_NODE *condition;
    AST_NODE **consequent;
    size_t body_count;
} IfStmtExpr;

typedef struct {
    AST_NODE *left;
    char *op;
    AST_NODE *right;
} LogicalExpr;

typedef struct AST_NODE {
    AST_TYPE type;

    union {
        NumericExpr numeric_expr;
        StringExpr string_expr;
        IdentifierExpr identifier_expr;
        BinaryExpr binary_expr;
        VarDecExpr var_dec_expr;
        IfStmtExpr if_stmt_expr;
        LogicalExpr logical_expr;
        BoolExpr bool_expr;
    } node;

} AST_NODE;

typedef struct {
    AST_NODE **body;
    size_t count;
} Program;

void print_ast(Program *ast);
void print_ast_node(AST_NODE *node, int indent_level);

#endif