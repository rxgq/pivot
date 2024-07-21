#include <stdlib.h>
#include <string.h>
#include "lexer.h"
#include "token.h"

void lexer_advance(LEXER *lexer) {
    lexer->curr++;
}

TOKEN* create_token(char *value, TokenType type) {
    TOKEN *token = (TOKEN *)malloc(sizeof(TOKEN));
    token->value = strdup(value);
    token->type = type;

    return token;
}

TOKEN *next_token(LEXER *lexer) {
    char c = lexer->source[lexer->curr];
    char c_buff[2];
    c_buff[0] = c;
    c_buff[1] = '\0';

    switch (c) {
        case '+': return create_token(c_buff, PLUS);
        case '-': return create_token(c_buff, MINUS);
        case '*': return create_token(c_buff, STAR);
        case '/': return create_token(c_buff, SLASH);

        default: return create_token("", BAD);
    }
}


TOKEN *tokenize(LEXER *lexer) {
    TOKEN *tokens = malloc(sizeof(TOKEN));

    int token_count = 0;
    while (lexer->source[lexer->curr] != '\0') {
        TOKEN token = *next_token(lexer);
        lexer_advance(lexer);

        token_to_str(token);
        token_count++;
    }

    return tokens;
}

LEXER *create_lexer(char *source) {
    LEXER *lexer = malloc(sizeof(LEXER));
    lexer->curr = 0;
    lexer->source = source;

    return lexer;
}
