#include "token.h"

Token init_token(TokenType type, char* value) {
    Token token;
    token.type = type;
    token.value = value;

    return token;
}