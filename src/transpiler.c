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

        case AST_BINARY_EXPR:
            generate_expression(node->node.binary_expr.left, output);
            fprintf(output, " %s ", node->node.binary_expr.op);
            generate_expression(node->node.binary_expr.right, output);
            break;

        default:
            fprintf(stderr, "Unknown expression type\n");
            exit(EXIT_FAILURE);
    }
}

void generate_var_dec(AST_NODE *node, FILE *output) {
    VarDecExpr *var_dec = &node->node.var_dec_expr;


    if (var_dec->type.type == STRING) {
        fprintf(output, "%s %s = ", "char*", var_dec->identifier);
    } else if (var_dec->type.type == INT) {
        fprintf(output, "%s %s = ", "int", var_dec->identifier);
    } else if (var_dec->type.type == CHR) {
        fprintf(output, "%s %s = ", "char", var_dec->identifier);
    } else if (var_dec->type.type == FLT) {
        fprintf(output, "%s %s = ", "float", var_dec->identifier);
    } else {
        fprintf(output, "%s %s = ", "UNKOWN*", var_dec->identifier);
    }

    generate_expression(var_dec->val, output);
    fprintf(output, ";\n");
}

void generate_stmt(AST_NODE *node, FILE *fptr) {
    switch (node->type) {
        case AST_VAR_DEC:
            generate_var_dec(node, fptr);
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

    fprintf(fptr, "#include <stdio.h>\n\nint main(int argc, char *argv[]){\n");

    for (size_t i = 0; i < ast->count; i++) {
        generate_stmt(ast->body[i], fptr);
    }

    fprintf(fptr, "return 0;\n}\n");
    fclose(fptr);
}
