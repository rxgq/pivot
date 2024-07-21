#include <stdio.h>
#include "token.h"

void token_to_str(TOKEN token) {
    printf("\nToken: %s %s", token.value, token_type_to_str(token.type));
}

char *token_type_to_str(TokenType type) {
    switch (type) {
        case PLUS: return "PLUS";
        case MINUS: return "MINUS";
        case STAR: return "STAR";
        case SLASH: return "SLASH";
        default: return "UNKNOWN";
    }
}
