#include "token.h"

#ifndef LEXER_H
#define LEXER_H

typedef struct {
    char *source;
    int curr;
} LEXER;

TOKEN *tokenize(LEXER *lexer);
LEXER *create_lexer(char *source);

#endif
