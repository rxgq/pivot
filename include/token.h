#ifndef TOKEN_H
#define TOKEN_H

typedef enum {
    PLUS, MINUS, STAR, SLASH, MODULO,
    BAD_TOKEN, EOF_TOKEN
} TokenType;

typedef struct {
    char *value;
    TokenType type;
} Token;

Token init_token(TokenType type, char* value);

#endif