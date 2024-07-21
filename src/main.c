#include <stdio.h>
#include <stdlib.h>

int main() {
    FILE *fptr = fopen("code.txt", "r");
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

    
}