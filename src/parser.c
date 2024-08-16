#include "parser.h"
#include "token.h"

Parser *init_parser(Token *tokens) {
    Parser *parser;
    parser->tokens = tokens;
    
}