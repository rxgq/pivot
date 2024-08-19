#ifndef TOKEN_H
#define TOKEN_H

typedef enum {
    PLUS, MINUS, STAR, SLASH, MODULO, PLUSEQ, MINUSEQ, STAREQ, SLASHEQ, MODULOEQ, 
    
    LBRACE, RBRACE, LPAREN, RPAREN, LSQBRACK, RSQBRACK, SEMICOLON, COMMA, DOT, COLON,
    LESST, GREATERT, LESSTEQ, GREATERTEQ, EQUALS, NEQUALS, ASSIGNMENT, NOT, OR, AND,

    IF, ELIF, ELSE, WHILE, FOR, LET, BREAK, CONTINUE, RETURN, PROC, TRUE_TOK, FALSE_TOK,
    INT, STR, CHAR, FLT, BOOL,

    ECHO,

    IDENTIFIER, NUMERIC, STRING,
    WHITESPACE, BAD_TOKEN, EOF_TOKEN
} TokenType;

typedef struct {
    const char *keyword;
    TokenType type;
} KeywordMap;

typedef struct {
    char *lexeme;
    TokenType type;
} Token;

Token init_token(TokenType type, char* lexeme);
char *print_token(TokenType type);

#endif