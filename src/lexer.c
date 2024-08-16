#include <stdio.h>
#include <stdlib.h>
#include "lexer.h"
#include "token.h"

void print(Lexer *lexer) {
    printf("\n[ ======= TOKENS ======= ]\n");
    for (int i = 0; i < lexer->count; i++) {
        printf("[%d: %s ]\n", i, lexer->tokens[i].value);
    }
}

Token next_token(char curr) {
    switch (curr) {
        case '+': return init_token(PLUS, "+");
        case '-': return init_token(MINUS, "-");
        case '*': return init_token(STAR, "*");
        case '/': return init_token(SLASH, "/");
        default:  return init_token(BAD_TOKEN, "BAD_TOKEN");
    }
}

Token *tokenize(Lexer *lexer) {
    while (lexer->source[lexer->current] != '\0') {
        char curr = lexer->source[lexer->current];
        Token token = next_token(curr);

        if (lexer->count >= lexer->capacity) {
            lexer->capacity *= 2;
            lexer->tokens = (Token *)realloc(lexer->tokens, lexer->capacity * sizeof(Token));
        }

        lexer->tokens[lexer->count++] = token;
        lexer->current++;
    }

    return lexer->tokens;
}

Lexer *init_lexer(char *source) {
    Lexer *lexer = (Lexer *)malloc(sizeof(Lexer));
    lexer->current = 0;
    lexer->source = source;
    lexer->count = 0;
    lexer->capacity = 1;
    lexer->tokens = (Token *)malloc(lexer->capacity * sizeof(Token));

    return lexer;
}
