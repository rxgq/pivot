#include <stdio.h>
#include <stdlib.h>
#include <sys/stat.h>
#include <sys/types.h>
#include "transpiler.h"

const char* ast_type_to_string(AST_TYPE type) {
    switch (type) {
        case AST_NUMERIC:
            return "AST_NUMERIC";
        case AST_STRING:
            return "AST_STRING";
        case AST_IDENTIFIER:
            return "AST_IDENTIFIER";
        case AST_BINARY_EXPR:
            return "AST_BINARY_EXPR";
        case AST_VAR_DEC:
            return "AST_VAR_DEC";
        case AST_IF_STMT:
            return "AST_IF_STMT";
        case AST_BOOL:
            return "AST_BOOL";
        case AST_LOGICAL_EXPR:
            return "AST_LOGICAL_EXPR";
        case AST_ASSIGNMENT_EXPR:
            return "ASSIGNMENT_EXPR";
        default:
            return "UNKNOWN_AST_TYPE";
    }
}

void generate_expression(AST_NODE *node, FILE *output) {
    switch (node->type) {
        case AST_NUMERIC:
            fprintf(output, "%s", node->node.numeric_expr.value);
            break;

        case AST_STRING:
            fprintf(output, "\"%s\"", node->node.string_expr.value);
            break;

        case AST_IDENTIFIER:
            fprintf(output, "%s", node->node.identifier_expr.value);
            break;

        case AST_BOOL:
            fprintf(output, "%s", node->node.bool_expr.value);
            break;

        case AST_BINARY_EXPR:
            generate_expression(node->node.binary_expr.left, output);
            fprintf(output, " %s ", node->node.binary_expr.op);
            generate_expression(node->node.binary_expr.right, output);
            break;

        case AST_LOGICAL_EXPR:
            generate_expression(node->node.logical_expr.left, output);
            if (strcmp(node->node.logical_expr.op, "or") == 0) {
                fprintf(output, " || ");
            } else if (strcmp(node->node.logical_expr.op, "and") == 0) {
                fprintf(output, " && ");    
            } else {
                fprintf(stderr, "Unknown logical operator: %s\n", node->node.logical_expr.op);
                exit(EXIT_FAILURE);
            }
            generate_expression(node->node.logical_expr.right, output);
            break;

        default:
            fprintf(stderr, "Unknown expression type\n");
            exit(EXIT_FAILURE);
    }
}

void generate_var_dec(AST_NODE *node, FILE *output) {
    VarDecExpr *expr = &node->node.var_dec_expr;

    if (expr->type.type == STRING) {
        fprintf(output, "%s %s = ", "char*", expr->identifier);
    } else if (expr->type.type == INT) {
        fprintf(output, "%s %s = ", "int", expr->identifier);
    } else if (expr->type.type == CHR) {
        fprintf(output, "%s %s = ", "char", expr->identifier);
    } else if (expr->type.type == FLT) {
        fprintf(output, "%s %s = ", "float", expr->identifier);
    } else {
        fprintf(output, "%s %s = ", "UNKOWN*", expr->identifier);
    }

    generate_expression(expr->val, output);
    fprintf(output, ";\n");
}

void generate_assignment_expr(AST_NODE *node, FILE *fptr) {
    AssignmentExpr *expr = &node->node.assignment_expr;

    fprintf(fptr, expr->identifier);
    fprintf(fptr, "=");
    generate_expression(expr->val, fptr);
    fprintf(fptr, ";\n");
}

void generate_if_stmt(AST_NODE *node, FILE *fptr) {
    IfStmtExpr *expr = &node->node.if_stmt_expr;

    fprintf(fptr, "if (");
    generate_expression(expr->condition, fptr);
    fprintf(fptr, ") ");

    fprintf(fptr, "{\n");
    for (size_t i = 0; i < expr->body_count; i++) {
        generate_stmt(expr->consequent[i], fptr);
    }
    fprintf(fptr, "}\n");
}

void generate_stmt(AST_NODE *node, FILE *fptr) {
    switch (node->type) {
        case AST_VAR_DEC:
            generate_var_dec(node, fptr);
            break;

        case AST_IF_STMT:
            generate_if_stmt(node, fptr);
            break;

        case AST_ASSIGNMENT_EXPR:
            generate_assignment_expr(node, fptr);
            break;

        default: fprintf(fptr, ast_type_to_string(node->type));
    }
}

void generate(Program *ast) {
    struct stat st = {0};
    if (stat("out", &st) == -1) {
        if (mkdir("out") != 0) {
            perror("Failed to create /out directory");
            exit(EXIT_FAILURE);
        }
    }

    FILE *fptr = fopen("out/output.c", "w");
    if (!fptr) {
        perror("Failed to open file in /out directory");
        exit(EXIT_FAILURE);
    }

    fprintf(fptr, "#include <stdio.h>\n#include <stdbool.h>\n\nint main(int argc, char *argv[]){\n");

    for (size_t i = 0; i < ast->count; i++) {
        generate_stmt(ast->body[i], fptr);
    }

    fprintf(fptr, "return 0;\n}\n");
    fclose(fptr);
}