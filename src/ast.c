#include <stdio.h>
#include <string.h>
#include "ast.h"
#include "token.h"

void print_bool_expr(BoolExpr *expr, int indent_level) {
    printf("%*sBoolExpr(%s)\n", indent_level, "", expr->value);
}

void print_numeric_expr(NumericExpr *expr, int indent_level) {
    printf("%*sNumericExpr(%s)\n", indent_level, "", expr->value);
}

void print_string_expr(StringExpr *expr, int indent_level) {
    printf("%*sStringExpr(%s)\n", indent_level, "", expr->value);
}

void print_identifier_expr(IdentifierExpr *expr, int indent_level) {
    printf("%*sIdentifierExpr(%s)\n", indent_level, "", expr->value);
}

void print_return_stmt(ReturnStmt *expr, int indent_level) {
    printf("%*sReturnStmt(\n", indent_level, "");
    print_ast_node(expr->expr, indent_level + 2);
    printf("%*s)\n", indent_level, "");
}

void print_comparison_expr(ComparisonExpr *expr, int indent_level) {
    printf("%*sComparisonExpr(\n", indent_level, "");
    print_ast_node(expr->left, indent_level + 2);
    printf("%*s%s\n", indent_level + 2, "", expr->op);
    print_ast_node(expr->right, indent_level + 2);
    printf("%*s)\n", indent_level, "");
}

void print_unary_expr(UnaryExpr *expr, int indent_level) {
    printf("%*sUnaryExpr(\n", indent_level, "");
    printf("%*sOperator: %s\n", indent_level + 2, "", expr->op);
    printf("%*sOperand:\n", indent_level + 2, "");
    print_ast_node(expr->operand, indent_level + 4);
    printf("%*s)\n", indent_level, "");
}

void print_assignment_expr(AssignmentExpr *expr, int indent_level) {
    printf("%*sAssignmentExpr(\n", indent_level, "");
    printf("%*sIdentifier: %s\n", indent_level + 2, "", expr->identifier);
    printf("%*sValue:\n", indent_level + 2, "");
    print_ast_node(expr->val, indent_level + 4);

    printf("%*s)\n", indent_level, "");
}

void print_if_stmt(IfStmtExpr *expr, int indent_level) {
    printf("%*sIfStmt(\n", indent_level, "");

    printf("%*sCondition:\n", indent_level + 2, "");
    print_ast_node(expr->condition, indent_level + 4);

    printf("%*sBody:\n", indent_level + 2, "");
    for (size_t i = 0; i < expr->body_count; i++) {
        print_ast_node(expr->consequent[i], indent_level + 4);
    }

    printf("%*s)\n", indent_level, "");
}

void print_logical_expr(LogicalExpr *expr, int indent_level) {
    printf("%*sLogicalExpr(\n", indent_level, "");
    print_ast_node(expr->left, indent_level + 2);
    printf("%*s%s\n", indent_level + 2, "", expr->op);
    print_ast_node(expr->right, indent_level + 2);
    printf("%*s)\n", indent_level, "");
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

void print_echo_expr(EchoExpr *expr, int indent_level) {
    printf("%*sEchoExpr(\n", indent_level, "");
    print_ast_node(expr->expr, indent_level + 2);
    printf("%*s)\n", indent_level, "");
}

void print_proc_param(ProcParam *param, int indent_level) {
    printf("%*sParam(\n", indent_level, "");
    printf("%*sIdentifier: ", indent_level + 2, "");
    printf(param->identifier, indent_level + 4, "");
    printf("%*sType: %s\n", indent_level + 2, "", param->type);
    printf("%*s)\n", indent_level, "");
}

void print_proc_stmt(ProcStmt *stmt, int indent_level) {
    printf("%*sProcStmt(\n", indent_level, "");
    printf("%*sIdentifier: ", indent_level + 2, "");
    print_ast_node(stmt->identifier, indent_level + 4);

    printf("%*sParameters:\n", indent_level + 2, "");
    for (size_t i = 0; i < stmt->parameter_count; i++) {
        print_ast_node(stmt->parameters[i], indent_level + 4);
    }

    printf("%*sReturnType: ", indent_level + 2, "");
    if (stmt->return_type) {
        printf(stmt->return_type, indent_level + 4);
    } else {
        printf("%*svoid\n", indent_level + 4, "");
    }

    printf("%*s\n  Body:\n", indent_level + 2, "");
    for (size_t i = 0; i < stmt->body_count; i++) {
        print_ast_node(stmt->consequent[i], indent_level + 4);
    }

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
        case AST_BOOL:
            print_bool_expr(&node->node.bool_expr, indent_level);
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
        case AST_IF_STMT:
            print_if_stmt(&node->node.if_stmt_expr, indent_level);
            break;
        case AST_LOGICAL_EXPR:
            print_logical_expr(&node->node.logical_expr, indent_level);
            break;
        case AST_ASSIGNMENT_EXPR:
            print_assignment_expr(&node->node.assignment_expr, indent_level);
            break;
        case AST_ECHO_EXPR:
            print_echo_expr(&node->node.echo_expr, indent_level);
            break;
        case AST_UNARY_EXPR:
            print_unary_expr(&node->node.unary_expr, indent_level);
            break;
        case AST_COMPARISON_EXPR:
            print_comparison_expr(&node->node.comparison_expr, indent_level);
            break;
        case AST_PROC_STMT:
            print_proc_stmt(&node->node.proc_stmt, indent_level);
            break;
        case AST_PROC_PARAM:
            print_proc_param(&node->node.proc_param, indent_level);
            break;
        case AST_RETURN_STMT:
            print_return_stmt(&node->node.return_stmt, indent_level);
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