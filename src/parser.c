#include <stdio.h>
#include <stdlib.h>
#include "lexer.h"
#include <string.h>

typedef struct {
    char name[64];
    int offset;
} Variable;

static Variable variables[256];
static int variable_count = 0;
static int stack_offset = 0;

static int find_variable(const char *name) {
    for (int i = 0; i < variable_count; i++) {
        if (strcmp(variables[i].name, name) == 0)
            return variables[i].offset;
    }

    printf("Undefined variable: %s\n", name);
    exit(1);
}

static int add_variable(const char *name) {
    stack_offset += 8;

    strcpy(variables[variable_count].name, name);
    variables[variable_count].offset = stack_offset;
    variable_count++;

    return stack_offset;
}

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
    if (current.type == TOKEN_IDENTIFIER) {
        int offset = find_variable(current.text);
        printf("    mov rax, [rbp-%d]\n", offset);
        advance();
        return;
    }

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

static void parse_statement(void) {
    if (current.type == TOKEN_LET) { // If we are creating a variable
        advance();

        if (current.type != TOKEN_IDENTIFIER) {
            printf("Expected variable name\n");
            exit(1);
        }

        // Copy variable name from token
        char name[64];
        strcpy(name, current.text);
        advance();

        expect(TOKEN_EQUALS);

        parse_expr();

        // Get the offset and put the variable's value there in memory using the offset
        int offset = add_variable(name);
        printf("    mov [rbp-%d], rax\n", offset);

        expect(TOKEN_SEMICOLON);
        return;
    }

    if (current.type == TOKEN_RETURN) {
        advance();

        parse_expr();

        expect(TOKEN_SEMICOLON);

        printf("    mov rsp, rbp\n");
        printf("    pop rbp\n");
        printf("    ret\n");
        return;
    }

    printf("Expected statement\n");
    exit(1);
}

// program: "return" -> expr -> ";"
void parse_program(void) {
    // Load first token
    advance();

    // Assembly boilerplate
    printf("global main\n");
    printf("section .text\n");
    printf("main:\n");

    // Allocate space for variables
    printf("    push rbp\n");
    printf("    mov rbp, rsp\n");
    printf("    sub rsp, 2048\n");

    // Execute each line of code until the end of the file (EOF)
    while (current.type != TOKEN_EOF) {
        parse_statement();
    }

    // printf("    mov rax, 0\n");
    // printf("    mov rsp, rbp\n");
    // printf("    pop rbp\n");
    printf("leave\n"); // free memory that we allocated automatically before returning
    printf("    ret\n");

    printf("section .note.GNU-stack noalloc noexec nowrite progbits\n");
}