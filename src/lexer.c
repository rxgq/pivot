#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include <string.h>
#include "lexer.h"
#include "token.h"

KeywordMap keyword_map[] = {
    {"if", IF},
    {"elif", ELIF},
    {"else", ELSE},
    {"while", WHILE},
    {"for", FOR},
    {"let", LET},
    {"true", TRUE_TOK},
    {"false", FALSE_TOK},
    {"and", AND},
    {"or", OR},
    {"not", NOT},
    {"int", INT},
    {"char", CHAR},
    {"flt", FLT},
    {"str", STRING},
    {"bool", BOOL},
    {"echo", ECHO},
    {"break", BREAK},
    {"continue", CONTINUE},
    {"proc", PROC},
    {"return", RETURN},
    {"END_KEYWORD", BAD_TOKEN}
};

void print_lexer(Lexer *lexer) {
    printf("\n[ ======= TOKENS ======= ]\n");

    for (int i = 0; i < lexer->count; i++) {
        printf("[ %s: %s ]\n", print_token(lexer->tokens[i].type), lexer->tokens[i].lexeme);
    }
}

void advance(Lexer *lexer) {
    lexer->current++;
}

TokenType check_keyword(const char *identifier) {
    for (int i = 0; strcmp(keyword_map[i].keyword, "END_KEYWORD") == 1; i++) {
        if (strcmp(keyword_map[i].keyword, identifier) == 0) {
            return keyword_map[i].type;
        }
    }

    return IDENTIFIER;
}


Token on_identifier(Lexer *lexer) {
    int start = lexer->current;

    while (isalpha(lexer->source[lexer->current])) {
        advance(lexer);
    }

    long len = lexer->current - start;
    char *identifier = (char *)malloc(len + 1);
    strncpy(identifier, lexer->source + start, len);
    identifier[len] = '\0';

    Token token;
    token.type = check_keyword(identifier);
    token.lexeme = identifier;
    return token;
}

Token on_numeric(Lexer *lexer) {
    int start = lexer->current;

    while (isdigit(lexer->source[lexer->current])) {
        advance(lexer);
    }

    if (lexer->source[lexer->current] == '.') {
        advance(lexer);

        while (isdigit(lexer->source[lexer->current])) {
            advance(lexer);
        }
    }

    long len = lexer->current - start;
    char *number = (char *)malloc(len + 1);
    strncpy(number, lexer->source + start, len);
    number[len] = '\0';

    Token token;
    token.type = NUMERIC;
    token.lexeme = number;
    return token;
}

Token on_string(Lexer *lexer) {
    advance(lexer); 
    int start = lexer->current;

    while (lexer->source[lexer->current] != '\"' && lexer->source[lexer->current] != '\0') {
        advance(lexer);
    }

    if (lexer->source[lexer->current] == '\"') {
        advance(lexer);
    } else {
        char *error_value = strdup("Unterminated string");
        return init_token(BAD_TOKEN, error_value);
    }

    long len = lexer->current - start - 1;
    char *string = (char *)malloc(len + 1);
    strncpy(string, lexer->source + start, len);
    string[len] = '\0';

    Token token;
    token.type = STRING;
    token.lexeme = string;
    return token;
}


Token on_double_tok(Lexer *lexer, char single, TokenType single_type, char next_char, TokenType double_type) {
    if (lexer->source[lexer->current] == single) {
        advance(lexer);

        if (lexer->source[lexer->current] == next_char) {
            advance(lexer);
            char *value = (char *)malloc(3);
            snprintf(value, 3, "%c%c", single, next_char);
            return init_token(double_type, value);
        }

        char *value = (char *)malloc(2);
        snprintf(value, 2, "%c", single);
        return init_token(single_type, value);
    }

    return init_token(BAD_TOKEN, "BAD_TOKEN");
}

Token on_single_tok(Lexer *lexer, TokenType type, char *tok) {
    advance(lexer);
    return init_token(type, tok);
}

Token next_token(Lexer *lexer) {
    char curr = lexer->source[lexer->current];

    while (isspace(curr)) {
        advance(lexer);
        curr = lexer->source[lexer->current];
    }

    if (curr == '\0') {
        return init_token(EOF_TOKEN, "EOF");
    }

    char *str = (char *)malloc(2 * sizeof(char));
    str[0] = curr;
    str[1] = '\0';

    switch (curr) {
        case '{': return on_single_tok(lexer, LBRACE, str);
        case '}': return on_single_tok(lexer, RBRACE, str);
        case '(': return on_single_tok(lexer, LPAREN, str);
        case ')': return on_single_tok(lexer, RPAREN, str);
        case ',': return on_single_tok(lexer, COMMA, str);
        case '.': return on_single_tok(lexer, DOT, str);
        case '[': return on_single_tok(lexer, LSQBRACK, str);
        case ']': return on_single_tok(lexer, RSQBRACK, str);
        case ';': return on_single_tok(lexer, SEMICOLON, str);
        case ':': return on_single_tok(lexer, COLON, str);
        case '+': return on_double_tok(lexer, '+', PLUS, '=', PLUSEQ);
        case '-': return on_double_tok(lexer, '-', MINUS, '=', MINUSEQ);
        case '*': return on_double_tok(lexer, '*', STAR, '=', STAREQ);
        case '/': return on_double_tok(lexer, '/', SLASH, '=', SLASHEQ);
        case '%': return on_double_tok(lexer, '%', MODULO, '=', MODULOEQ);
        case '<': return on_double_tok(lexer, '<', LESST, '=', LESSTEQ);
        case '>': return on_double_tok(lexer, '>', GREATERT, '=', GREATERTEQ);
        case '!': return on_double_tok(lexer, '!', NOT, '=', NEQUALS);
        case '=': return on_double_tok(lexer, '=', ASSIGNMENT, '=', EQUALS);
        case '\"': return on_string(lexer);
    }

    if (isalpha(curr)) return on_identifier(lexer);
    else if (isdigit(curr)) return on_numeric(lexer);

    advance(lexer);
    return init_token(BAD_TOKEN, "BAD_TOKEN");
}

Token *tokenize(Lexer *lexer) {
    while (lexer->source[lexer->current] != '\0') {
        Token token = next_token(lexer);

        if (lexer->count >= lexer->capacity) {
            lexer->capacity *= 2;
            lexer->tokens = (Token *)realloc(lexer->tokens, lexer->capacity * sizeof(Token));
        }

        lexer->tokens[lexer->count++] = token;
    }

    if (lexer->count >= lexer->capacity) {
        lexer->capacity *= 2;
        lexer->tokens = (Token *)realloc(lexer->tokens, lexer->capacity * sizeof(Token));
    }
    lexer->tokens[lexer->count++] = init_token(EOF_TOKEN, "EOF");


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