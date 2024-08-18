#include <stdio.h>
#include <string.h>
#include "ast.h"
#include "token.h"

void print_numeric_expr(NumericExpr *expr, int indent_level) {
    printf("%*sNumericExpr(%s)\n", indent_level, "", expr->value);
}

void print_string_expr(StringExpr *expr, int indent_level) {
    printf("%*sStringExpr(%s)\n", indent_level, "", expr->value);
}

void print_identifier_expr(IdentifierExpr *expr, int indent_level) {
    printf("%*sIdentifierExpr(%s)\n", indent_level, "", expr->value);
}

void print_binary_expr(BinaryExpr *expr, int indent_level) {
    printf("%*sBinaryExpr(\n", indent_level, "");
    print_ast_node(expr->left, indent_level + 2);
    printf("%*s%s\n", indent_level + 2, "", expr->op);
    print_ast_node(expr->right, indent_level + 2);
    printf("%*s)\n", indent_level, "");
}

void print_var_dec(VarDecExpr *expr, int indent_level) {
    printf("%*sVarDecExpr(\n", indent_level, "");
    printf("%*sIdentifier: %s\n", indent_level + 2, "", expr->identifier);
    printf("%*sType: %s\n", indent_level + 2, "", print_token( expr->type.type));
    printf("%*sValue:\n", indent_level + 2, "");
    print_ast_node(expr->val, indent_level + 4);
    printf("%*s)\n", indent_level, "");
}

void print_ast_node(AST_NODE *node, int indent_level) {
    if (!node) {
        printf("%*sNULL\n", indent_level, "");
        return;
    }

    switch (node->type) {
        case AST_NUMERIC:
            print_numeric_expr(&node->node.numeric_expr, indent_level);
            break;
        case AST_STRING:
            print_string_expr(&node->node.string_expr, indent_level);
            break;
        case AST_IDENTIFIER:
            print_identifier_expr(&node->node.identifier_expr, indent_level);
            break;
        case AST_BINARY_EXPR:
            print_binary_expr(&node->node.binary_expr, indent_level);
            break;
        case AST_VAR_DEC:
            print_var_dec(&node->node.var_dec_expr, indent_level);
            break;

        default:
            printf("%*sUnknown AST_NODE type\n", indent_level, "");
            break;
    }
}

void print_ast(Program *program) {
    if (!program) {
        printf("Program is NULL\n");
        return;
    }
    printf("\nProgram (%zu):\n", program->count);
    for (size_t i = 0; i < program->count; ++i) {
        printf("Statement %zu:\n", i + 1);
        print_ast_node(program->body[i], 2);
    }
    printf("\n");
}