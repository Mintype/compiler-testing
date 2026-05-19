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

        return (Token){TOKEN_NUMBER, value, ""};
    }

    if (isalpha(src[pos]) || src[pos] == '_') {
        char text[64];
        int len = 0;

        while (isalnum(src[pos]) || src[pos] == '_') {
            if (len < 63)
                text[len++] = src[pos];
            pos++;
        }

        text[len] = '\0';

        if (strcmp(text, "return") == 0)
            return (Token){TOKEN_RETURN, 0, ""};

        if (strcmp(text, "let") == 0)
            return (Token){TOKEN_LET, 0, ""};

        Token tok = {TOKEN_IDENTIFIER, 0, ""};
        strcpy(tok.text, text);
        return tok;
    }

    // equal equal (==)
    if (src[pos] == '=' && src[pos + 1] == '=') {
        pos += 2;
        return (Token){TOKEN_EQEQ, 0, ""};
    }

    // not equal (!=)
    if (src[pos] == '!' && src[pos + 1] == '=') {
        pos += 2;
        return (Token){TOKEN_NEQ, 0, ""};
    }

    // less than
    if (src[pos] == '=') {
        pos++;
        return (Token){TOKEN_LT, 0, ""};
    }

    // greater than
    if (src[pos] == '=') {
        pos++;
        return (Token){TOKEN_GT, 0, ""};
    }

    // less than or equal (==)
    if (src[pos] == '<' && src[pos + 1] == '=') {
        pos += 2;
        return (Token){TOKEN_LTEQ, 0, ""};
    }

    // greater than or equal (==)
    if (src[pos] == '>' && src[pos + 1] == '=') {
        pos += 2;
        return (Token){TOKEN_GTEQ, 0, ""};
    }

    if (src[pos] == '=') {
        pos++;
        return (Token){TOKEN_EQUALS, 0, ""};
    }

    if (src[pos] == '+') {
        pos++;
        return (Token){TOKEN_PLUS, 0, ""};
    }

    if (src[pos] == '-') {
        pos++;
        return (Token){TOKEN_MINUS, 0, ""};
    }

    if (src[pos] == '*') {
        pos++;
        return (Token){TOKEN_TIMES, 0, ""};
    }

    if (src[pos] == '/') {
        pos++;
        return (Token){TOKEN_DIVIDES, 0, ""};
    }

    if (src[pos] == '(') {
        pos++;
        return (Token){TOKEN_LPAREN, 0, ""};
    }

    if (src[pos] == ')') {
        pos++;
        return (Token){TOKEN_RPAREN, 0, ""};
    }

    if (src[pos] == ';') {
        pos++;
        return (Token){TOKEN_SEMICOLON, 0, ""};
    }

    return (Token){TOKEN_EOF, 0, ""};
}