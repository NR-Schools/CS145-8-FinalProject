#pragma once

#include <string>

enum TokenType
{
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

typedef struct token
{
    enum TokenType type;
    std::string lexeme = "";

    token() {}
    token(TokenType type) {
        this->type = type;
    }
    token(TokenType type, std::string lexeme) {
        this->type = type;
        this->lexeme = lexeme;
    }
} Token;