#ifndef AST_H
#define AST_H

#include "token.h"

typedef struct AST_NODE AST_NODE;

typedef enum {
    AST_NUMERIC,
    AST_STRING,
    AST_BOOL, // true / false
    AST_BOOL_DEC, // let x: bool = false;
    AST_IDENTIFIER,
    AST_BINARY_EXPR,
    AST_VAR_DEC,
    AST_IF_STMT,
    AST_LOGICAL_EXPR,
    AST_ASSIGNMENT_EXPR,
    AST_ECHO_EXPR,
    AST_UNARY_EXPR,
    AST_COMPARISON_EXPR,
    AST_WHILE_STMT,
    AST_LOOP_STMT
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
    char *value;
} BoolDecExpr;

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
    AST_NODE *condition;
    AST_NODE **consequent;
    size_t body_count;
} WhileStmtExpr;

typedef struct {
    AST_NODE *left;
    char *op;
    AST_NODE *right;
} LogicalExpr;

typedef struct {
    char *identifier;
    AST_NODE *val;
} AssignmentExpr;

typedef struct {
    AST_NODE *expr;
} EchoExpr;

typedef struct {
    char *op;
    AST_NODE *operand;
} UnaryExpr;

typedef struct {
    AST_NODE *left;
    char *op;
    AST_NODE *right;
} ComparisonExpr;

typedef struct {
    char *stmt;
} LoopStmt;

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
        AssignmentExpr assignment_expr;
        BoolExpr bool_expr;
        EchoExpr echo_expr;
        BoolDecExpr bool_dec_expr;
        UnaryExpr unary_expr;
        ComparisonExpr comparison_expr;
        WhileStmtExpr while_stmt_expr;
        LoopStmt loop_stmt;
    } node;

} AST_NODE;

typedef struct {
    AST_NODE **body;
    size_t count;
} Program;

void print_ast(Program *ast);
void print_ast_node(AST_NODE *node, int indent_level);

#endif