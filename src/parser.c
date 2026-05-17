#include <stdio.h>
#include <stdlib.h>
#include "lexer.h"

// Current token the parser is looking at
static Token current;

// Move to the next token from the lexer
static void advance(void) {
    current = next_token();
}

// Check if we have the correct token type
// If token is wrong -> parser error
static void expect(TokenType type) {
    if (current.type != type) {
        printf("Unexpected token\n");
        exit(1);
    }
    advance();
}

static void parse_expr(void);

// Handles:
// - numbers
// - parenthesized expressions
static void parse_factor(void) {
    if (current.type == TOKEN_NUMBER) {
        printf("    mov rax, %d\n", current.value); // Put number into return register
        advance();
        return;
    }

    if (current.type == TOKEN_LPAREN) {
        advance();
        parse_expr(); // recursively parse inside parentheses
        expect(TOKEN_RPAREN);
        return;
    }

    printf("Expected number or (\n");
    exit(1);
}

// Handles:
// - multiplication
// - division
static void parse_term(void) {
    parse_factor(); // Parse left side first

    while (current.type == TOKEN_TIMES || current.type == TOKEN_DIVIDES) {
        TokenType op = current.type;
        advance();

        printf("    push rax\n"); // Save left side result
        parse_factor(); // Parse right side
        printf("    pop rbx\n"); // Restore left side into rbx

        if (op == TOKEN_TIMES) {
            printf("    imul rax, rbx\n");
        } else {
            printf("    mov rcx, rax\n");
            printf("    mov rax, rbx\n");
            printf("    cqo\n");
            printf("    idiv rcx\n");
        }
    }
}

// Handles:
// - addition
// - subtraction
static void parse_expr(void) {
    parse_term(); // Parse left side first

    while (current.type == TOKEN_PLUS || current.type == TOKEN_MINUS) {
        TokenType op = current.type;
        advance();

        
        printf("    push rax\n"); // Save left side result
        parse_term(); // Parse right side
        printf("    pop rbx\n"); // Restore left side into rbx

        if (op == TOKEN_PLUS) {
            printf("    add rax, rbx\n");
        } else {
            printf("    sub rbx, rax\n");
            printf("    mov rax, rbx\n");
        }
    }
}

// program: "return" -> expr -> ";"
void parse_program(void) {
    // Load first token
    advance();

    // Assembly boilerplate
    printf("global main\n");
    printf("section .text\n");
    printf("main:\n");

    expect(TOKEN_RETURN);
    parse_expr();
    expect(TOKEN_SEMICOLON);

    printf("    ret\n");
    printf("section .note.GNU-stack noalloc noexec nowrite progbits\n");
}