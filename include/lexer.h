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
    TOKEN_SEMICOLON,
    TOKEN_EOF,
} TokenType;

typedef struct {
    TokenType type;
    int value;
} Token;

void lexer_init(char *source);
Token next_token(void);

#endif