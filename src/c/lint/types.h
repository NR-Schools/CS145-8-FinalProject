#pragma once

enum TokenType {
    // Allowed Special Tokens
    TOKEN_SEPARATOR,
    
    // Operators
    TOKEN_OPERATOR,

    // Identifier
    TOKEN_RESERVED,
    TOKEN_IDENTIFIER,

    // Literals
    TOKEN_INTEGER,
    TOKEN_DECIMAL,

    // Invalid Token
    TOKEN_INVALID,

    // END
    TOKEN_END
};

typedef struct {
    enum TokenType type;
    char lexeme[50];
} Token;