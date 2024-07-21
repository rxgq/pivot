#ifndef TOKEN_H
#define TOKEN_H

typedef enum {
    PLUS, MINUS, STAR, SLASH,
    EQ, ASSIGNMENT, LPAREN, RPAREN, LBRACE, RBRACE,
    LT, GT, LTE, GTE, NEQ, NOT,
    BAD, WHITESPACE,
} TokenType;

typedef struct {
    char *value;
    TokenType type;
} TOKEN;

void token_to_str(TOKEN token);
char *token_type_to_str(TokenType type);

#endif