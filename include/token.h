#ifndef TOKEN_H
#define TOKEN_H

typedef enum {
    PLUS, MINUS, STAR, SLASH, MODULO, PLUSEQ, MINUSEQ, STAREQ, SLASHEQ, MODULOEQ, 
    LBRACE, RBRACE, LPAREN, RPAREN, LSQBRACK, RSQBRACK, SEMICOLON, COMMA, DOT,
    LESST, GREATERT, LESSTEQ, GREATERTEQ, EQUALS, NEQUALS, ASSIGNMENT, NOT, OR, AND,

    IDENTIFIER, NUMBER, STRING,
    WHITESPACE, BAD_TOKEN, EOF_TOKEN
} TokenType;

typedef struct {
    char *value;
    TokenType type;
} Token;

Token init_token(TokenType type, char* value);

#endif