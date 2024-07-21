#ifndef TOKEN_H
#define TOKEN_H

enum {
    PLUS, MINUS, STAR, SLASH
} TokenType;

typedef struct {
    char *value;
    enum TokenType type;
} TOKEN;

#endif