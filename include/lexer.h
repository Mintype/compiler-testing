#ifndef LEXER_H
#define LEXER_H

typedef enum {
    TOKEN_RETURN,
    TOKEN_NUMBER,
    TOKEN_PLUS,
    TOKEN_MINUS,
    TOKEN_TIMES,
    TOKEN_DIVIDES,
    TOKEN_LPAREN,
    TOKEN_RPAREN,
    TOKEN_LET,
    TOKEN_IDENTIFIER,
    TOKEN_EQUALS,
    TOKEN_SEMICOLON,
    TOKEN_EOF,
} TokenType;

typedef struct {
    TokenType type;
    int value;
    char text[64];
} Token;

void lexer_init(char *source);
Token next_token(void);

#endif