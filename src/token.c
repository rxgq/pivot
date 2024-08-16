#include "token.h"

Token init_token(TokenType type, char* lexeme) {
    Token token;
    token.type = type;
    token.lexeme = lexeme;

    return token;
}