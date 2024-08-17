#include "token.h"

#ifndef LEXER_H
#define LEXER_H

typedef struct {
    int current;
    char *source;
    Token* tokens;
    int count;
    int capacity;
} Lexer;

Lexer *init_lexer(char *source);
Token *tokenize(Lexer *lexer);
void print_lexer(Lexer *lexer);

#endif