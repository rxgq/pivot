#ifndef TOKEN_H
#define TOKEN_H

typedef enum {
    PLUS, MINUS, STAR, SLASH,
    BAD,
} TokenType;

typedef struct {
    char *value;
    TokenType type;
} TOKEN;

void token_to_str(TOKEN token);
char *token_type_to_str(TokenType type);

#endif