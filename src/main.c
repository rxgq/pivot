#include <stdio.h>
#include <stdlib.h>
#include "lexer.h"
#include "ast.h"

int main() {
    FILE *fptr = fopen("example/code.txt", "r");
    char *buff;

    if (!fptr) {
        perror("Error reading file");
        return 1;
    }

    fseek(fptr, 0, SEEK_END);
    long sz = ftell(fptr);
    rewind(fptr);

    buff = (char *)malloc((sz + 1) * sizeof(char));
    fread(buff, 1, sz, fptr);
    buff[sz] = '\0';

    fclose(fptr);
    
    LEXER *lexer = create_lexer(buff);
    TOKEN* tokens = tokenize(lexer);

    PARSER *parser = create_parser(tokens);
    parse_ast(parser);


    free(buff);
}