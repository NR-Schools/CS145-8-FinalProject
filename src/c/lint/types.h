#pragma once

enum TokenType {
    // Single Character Token
    TOKEN_LPAREN, TOKEN_RPAREN, TOKEN_COLON, TOKEN_SEMI_COLON,
    TOKEN_ADD, TOKEN_SUBTRACT,

    // Multi Character Token
    TOKEN_DOUBLE_LT, TOKEN_ASSIGN,
    TOKEN_GT, TOKEN_LT, TOKEN_EQ,

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

struct Token {
    enum TokenType type;
    char lexeme[50];
};