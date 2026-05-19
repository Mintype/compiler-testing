#ifndef LEXER_H
#define LEXER_H

typedef enum {
    TOKEN_RETURN,       // return
    TOKEN_NUMBER,       // a number
    TOKEN_PLUS,         // +
    TOKEN_MINUS,        // -
    TOKEN_TIMES,        // *
    TOKEN_DIVIDES,      // /
    TOKEN_LPAREN,       // (
    TOKEN_RPAREN,       // )
    TOKEN_LET,          // let
    TOKEN_IDENTIFIER,   // variable name
    TOKEN_EQUALS,       // =
    TOKEN_EQEQ,         // ==
    TOKEN_NEQ,          // !=
    TOKEN_LT,           // <
    TOKEN_GT,           // >
    TOKEN_LTEQ,         // <=
    TOKEN_GTEQ,         // >=
    TOKEN_SEMICOLON,    // ;
    TOKEN_EOF,          // end of file
} TokenType;

typedef struct {
    TokenType type;
    int value;
    char text[64];
} Token;

void lexer_init(char *source);
Token next_token(void);

#endif