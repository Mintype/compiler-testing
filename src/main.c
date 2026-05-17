#include <stdio.h>
#include <stdlib.h>
#include "lexer.h"

char *read_file(const char *path) {
    FILE *file = fopen(path, "rb");
    if (!file) {
        perror("fopen");
        exit(1);
    }

    fseek(file, 0, SEEK_END);
    long size = ftell(file);
    rewind(file);

    char *buffer = malloc(size + 1); // + 1 is for the null byte
    fread(buffer, 1, size, file);
    buffer[size] = '\0';

    fclose(file);
    return buffer;
}

int main(int argc, char **argv) {
    if (argc != 2) {
        printf("usage: compiler <file>\n");
        return 1;
    }

    char *source = read_file(argv[1]);

    lexer_init(source);

    Token tok;
    while ((tok = next_token()).type != TOKEN_EOF) {
        if (tok.type == TOKEN_RETURN)
            printf("TOKEN_RETURN\n");
        else if (tok.type == TOKEN_NUMBER)
            printf("TOKEN_NUMBER(%d)\n", tok.value);
        else if (tok.type == TOKEN_PLUS)
            printf("TOKEN_PLUS\n");
        else if (tok.type == TOKEN_MINUS)
            printf("TOKEN_MINUS\n");
        else if (tok.type == TOKEN_TIMES)
            printf("TOKEN_TIMES\n");
        else if (tok.type == TOKEN_DIVIDES)
            printf("TOKEN_DIVIDES\n");
        else if (tok.type == TOKEN_SEMICOLON)
            printf("TOKEN_SEMICOLON\n");
    }

    free(source);
    return 0;
}