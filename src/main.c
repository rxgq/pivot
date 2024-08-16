#include <stdio.h>
#include <stdlib.h>
#include "lexer.h"

int main(int argc, char *argv[]) {
    if (argc < 2) {
        printf("Usage: ./main.exe <path>");
        return 1;
    }

    FILE *fptr = fopen(argv[1], "r");
    if (!fptr) {
        perror("File path was not found");
        return 1;
    }

    fseek(fptr, 0, SEEK_END);
    long sz = ftell(fptr);
    rewind(fptr);

    char *buffer = (char *)malloc(sz + 1);
    if (!buffer) {
        perror("Memory allocation failed");
        fclose(fptr);
        return 1;
    }

    fread(buffer, 1, sz, fptr);
    buffer[sz] = '\0';
    fclose(fptr);

    Lexer *lexer = init_lexer(buffer);
    tokenize(lexer);
    print(lexer);

    free(buffer);
    return 0;
}