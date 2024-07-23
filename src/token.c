#include <stdio.h>
#include "token.h"

void token_to_str(TOKEN token) {
    printf("\nToken: >%s< %s", token.value, token_type_to_str(token.type));
}

char *token_type_to_str(TokenType type) {
    switch (type) {
        case PLUS: return "PLUS";
        case MINUS: return "MINUS";
        case STAR: return "STAR";
        case SLASH: return "SLASH";
        case EQ: return "EQ";
        case LT: return "LT";
        case GT: return "GT";
        case GTE: return "GTE";
        case LTE: return "LTE";
        case NEQ: return "NEQ";
        case NOT: return "NOT";
        case LPAREN: return "LPAREN";
        case RPAREN: return "RPAREN";
        case LBRACE: return "LBRACE";
        case RBRACE: return "RBRACE";
        case INT: return "INT";
        case IDENTIFIER: return "IDENTIFIER";
        case ASSIGNMENT: return "ASSIGNMENT";
        case WHITESPACE: return "WHITESPACE";
        case IF: return "IF";
        case ELIF: return "ELIF";
        case ELSE: return "ELSE";
        case FLOAT: return "FLOAT";
        default: return "UNKNOWN";
    }
}
