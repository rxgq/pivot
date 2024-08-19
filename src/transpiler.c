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
        case AST_BOOL_DEC:
            return "AST_BOOL_DEC";
        default:
            return "UNKNOWN_AST_TYPE";
    }
}

void generate_comparison_expr(AST_NODE *node, FILE *fptr) {
    ComparisonExpr *expr = &node->node.comparison_expr;

    if (expr->left->type == AST_STRING || expr->right->type == AST_STRING) {
        fprintf(fptr, "strcmp(");
        generate_expression(expr->left, fptr);
        fprintf(fptr, ",");
        generate_expression(expr->right, fptr);
        fprintf(fptr, ")==");
        if (strcmp(expr->op, "==") == 0) {
            fprintf(fptr, "0");
        } else if (strcmp(expr->op, "!=") == 0) {
            fprintf(fptr, "1");
        }
        return;
    }

    generate_expression(expr->left, fptr);
    fprintf(fptr, "%s", expr->op);
    generate_expression(expr->right, fptr);
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

        case AST_BOOL_DEC:
            fprintf(output, "%s", node->node.bool_expr.value);
            break;

        case AST_UNARY_EXPR:
            if (strcmp(node->node.unary_expr.op, "not") == 0) {
                fprintf(output, "%s", "!");
            }
            generate_expression(node->node.unary_expr.operand, output);
            break;

        case AST_BINARY_EXPR:
            generate_expression(node->node.binary_expr.left, output);
            fprintf(output, "%s", node->node.binary_expr.op);
            generate_expression(node->node.binary_expr.right, output);
            break;

        case AST_LOGICAL_EXPR:
            generate_expression(node->node.logical_expr.left, output);
            if (strcmp(node->node.logical_expr.op, "or") == 0) {
                fprintf(output, "||");
            } else if (strcmp(node->node.logical_expr.op, "and") == 0) {
                fprintf(output, "&&");
            } else {
                fprintf(stderr, "Unknown logical operator: %s\n", node->node.logical_expr.op);
                exit(EXIT_FAILURE);
            }
            generate_expression(node->node.logical_expr.right, output);
            break;

        case AST_COMPARISON_EXPR:
            generate_comparison_expr(node, output);
            break;

        case AST_PROC_CALL:
            generate_proc_call(node, output);
            break;

        default:
            fprintf(stderr, "Unknown expression type\n");
            exit(EXIT_FAILURE);
    }
}

void generate_var_dec(AST_NODE *node, FILE *output) {
    VarDecExpr *expr = &node->node.var_dec_expr;

    if (expr->type.type == STRING) {
        fprintf(output, "%s %s=", "char*", expr->identifier);
    } else if (expr->type.type == INT) {
        fprintf(output, "%s %s=", "int", expr->identifier);
    } else if (expr->type.type == CHAR) {
        fprintf(output, "%s %s=", "char", expr->identifier);
    } else if (expr->type.type == FLT) {
        fprintf(output, "%s %s=", "float", expr->identifier);
    } else if (expr->type.type == BOOL) {
        fprintf(output, "%s %s=", "bool", expr->identifier);
    } else {
        fprintf(output, "%s %s=", "UNKOWN*", expr->identifier);
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

void generate_while_stmt(AST_NODE *node, FILE *fptr) {
    WhileStmtExpr *expr = &node->node.while_stmt_expr;

    fprintf(fptr, "while(");
    generate_expression(expr->condition, fptr);
    fprintf(fptr, ")");

    fprintf(fptr, "{\n");
    for (size_t i = 0; i < expr->body_count; i++) {
        generate_stmt(expr->consequent[i], fptr);
    }
    fprintf(fptr, "}\n");
}

void generate_if_stmt(AST_NODE *node, FILE *fptr) {
    IfStmtExpr *expr = &node->node.if_stmt_expr;

    fprintf(fptr, "if(");
    generate_expression(expr->condition, fptr);
    fprintf(fptr, ")");

    fprintf(fptr, "{\n");
    for (size_t i = 0; i < expr->body_count; i++) {
        generate_stmt(expr->consequent[i], fptr);
    }
    fprintf(fptr, "}\n");
}

void generate_echo_expr(AST_NODE *node, FILE *fptr) {
    fprintf(fptr, "printf(");
    generate_expression(node->node.echo_expr.expr, fptr);
    fprintf(fptr, ");\n");
}

void generate_simple_stmt(AST_NODE *node, FILE *fptr) {
    LoopStmt *stmt = &node->node.loop_stmt;
    fprintf(fptr, stmt->stmt);
    fprintf(fptr, ";\n");
}

void generate_return_stmt(AST_NODE *node, FILE *fptr) {
    ReturnStmt *stmt = &node->node.return_stmt;
    fprintf(fptr, "return ");
    generate_expression(stmt->expr, fptr);
    fprintf(fptr, ";");
}

void generate_proc_stmt(AST_NODE *node, FILE *fptr) {
    ProcStmt *proc = &node->node.proc_stmt;

    fprintf(fptr, "%s %s(", proc->return_type, proc->identifier->node.identifier_expr.value);
    for (int i = 0; i < proc->parameter_count; i++) {
        ProcParam param = proc->parameters[i]->node.proc_param;

        fprintf(fptr, "%s %s", param.type, param.identifier);
        if (i < proc->parameter_count - 1) {
            fprintf(fptr, ", ");
        }
    }
    fprintf(fptr, "){\n");

    for (int i = 0; i < proc->body_count; i++) {
        generate_stmt(proc->consequent[i], fptr);
    }

    fprintf(fptr, "}\n\n");
}

void generate_proc_call(AST_NODE *node, FILE *fptr) {
    ProcCall *call = &node->node.proc_call;

    fprintf(fptr, call->identifier);
    fprintf(fptr, "(");

    for (int i = 0; i < call->param_count; i++) {
        AST_NODE* param = call->params[i];
        generate_expression(param, fptr);

        if (i < call->param_count - 1) {
            fprintf(fptr, ", ");
        }
    }
    fprintf(fptr, ");");
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

        case AST_ECHO_EXPR:
            generate_echo_expr(node, fptr);
            break;

        case AST_WHILE_STMT:
            generate_while_stmt(node, fptr);
            break;

        case AST_LOOP_STMT:
            generate_simple_stmt(node, fptr);
            break;

        case AST_RETURN_STMT:
            generate_return_stmt(node, fptr);
            break;

        case AST_PROC_STMT:
            generate_proc_stmt(node, fptr);
            break;

        case AST_PROC_CALL:
            generate_proc_call(node, fptr);
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

    fprintf(fptr, "#include <stdio.h>\n#include <stdbool.h>\n#include <string.h>\n\n");

    for (size_t i = 0; i < ast->count; i++) {
        if (ast->body[i]->type == AST_PROC_STMT) {
            generate_proc_stmt(ast->body[i], fptr);
        }
    }

    fprintf(fptr, "int main(int argc, char *argv[]){\n");
    for (size_t i = 0; i < ast->count; i++) {
        if (ast->body[i]->type != AST_PROC_STMT) {
            generate_stmt(ast->body[i], fptr);
        }
    }

    fprintf(fptr, "return 0;\n}\n");
    fclose(fptr);
}