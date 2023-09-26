#pragma once

enum TokenType {
    // Allowed Special Tokens
    TOKEN_LPAREN,
    TOKEN_RPAREN,
    TOKEN_COLON,
    TOKEN_SEMI_COLON,
    
    // Operators
    TOKEN_DOUBLE_LT,
    TOKEN_ASSIGN,
    TOKEN_GT,
    TOKEN_LT,
    TOKEN_EQ,
    TOKEN_GTE,
    TOKEN_LTE,
    TOKEN_NE,
    TOKEN_ADD,
    TOKEN_SUBTRACT,

    // Identifier
    TOKEN_RESERVED,
    TOKEN_IDENTIFIER,

    // Literals
    TOKEN_NUMBER,

    // Invalid Token
    TOKEN_INVALID,

    // END
    TOKEN_END
};

typedef struct {
    enum TokenType type;
    char lexeme[50];
} Token;