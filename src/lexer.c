#include <stdlib.h>
#include <string.h>
#include "lexer.h"
#include "token.h"

void lexer_advance(LEXER *lexer) {
    lexer->curr++;
}

char peek(LEXER *lexer) {
    return lexer->source[lexer->curr + 1];
}

int match(LEXER *lexer, char c) {
    if (peek(lexer) == c) {
        lexer_advance(lexer);
        return 0;
    }

    return 1;
}

TOKEN* create_token(char *value, TokenType type) {
    TOKEN *token = (TOKEN *)malloc(sizeof(TOKEN));
    token->value = strdup(value);
    token->type = type;

    return token;
}

TOKEN *on_number() {

}

TOKEN *next_token(LEXER *lexer) {
    char c = lexer->source[lexer->curr];
    char c_buff[2] = { c, '\0' };

    switch (c) {
        case '+': return create_token(c_buff, PLUS);
        case '-': return create_token(c_buff, MINUS);
        case '*': return create_token(c_buff, STAR);
        case '/': return create_token(c_buff, SLASH);
        case '(': return create_token(c_buff, LPAREN);
        case ')': return create_token(c_buff, RPAREN);
        case '{': return create_token(c_buff, LBRACE);
        case '}': return create_token(c_buff, RBRACE);

        case '>': {
            if (peek(lexer) == '=') {
                lexer_advance(lexer);
                return create_token(">=", GTE);
            }
            return create_token(c_buff, GT);
        }

        case '<': {
            if (peek(lexer) == '=') {
                lexer_advance(lexer);
                return create_token("<=", LTE);
            }
            return create_token(c_buff, LT);
        }

        case '!': {
            if (peek(lexer) == '=') {
                lexer_advance(lexer);
                return create_token("!=", NEQ);
            }
            return create_token(c_buff, NOT);
        }
        case '=': {
            if (peek(lexer) == '=') {
                lexer_advance(lexer);
                return create_token("==", EQ);
            }
            return create_token(c_buff, ASSIGNMENT);
        }

        case ' ': return create_token(" ", WHITESPACE);
        default:  return create_token("", BAD);
    }
}

TOKEN *tokenize(LEXER *lexer) {
    TOKEN *tokens = malloc(sizeof(TOKEN));

    int token_count = 0;
    while (lexer->source[lexer->curr] != '\0') {
        TOKEN token = *next_token(lexer);
        lexer_advance(lexer);

        token_count++;
        token_to_str(token);
    }

    return tokens;
}

LEXER *create_lexer(char *source) {
    LEXER *lexer = malloc(sizeof(LEXER));
    lexer->curr = 0;
    lexer->source = source;

    return lexer;
}
