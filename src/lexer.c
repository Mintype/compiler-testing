#include <ctype.h>
#include <string.h>
#include "lexer.h"

static char *src;
static int pos = 0;

void lexer_init(char *source) {
    src = source;
    pos = 0;
}

Token next_token(void) {
    while (src[pos] == ' ' || src[pos] == '\n' || src[pos] == '\t')
        pos++;

    if (isdigit(src[pos])) {
        int value = 0;

        while (isdigit(src[pos])) {
            value = value * 10 + (src[pos] - '0');
            pos++;
        }

        return (Token){TOKEN_NUMBER, value};
    }

    if (strncmp(&src[pos], "return", 6) == 0) {
        pos += 6;
        return (Token){TOKEN_RETURN, 0};
    }

    if (src[pos] == '+') {
        pos++;
        return (Token){TOKEN_PLUS, 0};
    }

    if (src[pos] == '-') {
        pos++;
        return (Token){TOKEN_MINUS, 0};
    }

    if (src[pos] == '*') {
        pos++;
        return (Token){TOKEN_TIMES, 0};
    }

    if (src[pos] == '/') {
        pos++;
        return (Token){TOKEN_DIVIDES, 0};
    }

    if (src[pos] == ';') {
        pos++;
        return (Token){TOKEN_SEMICOLON, 0};
    }

    return (Token){TOKEN_EOF, 0};
}