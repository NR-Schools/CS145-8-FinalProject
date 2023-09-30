#pragma once

#include <vector>
#include <string>

#include "token.hpp"


class Parser
{
public:
    Parser(const std::vector<Token>& tokens);
    void parse();

private:
    std::vector<Token> tokens;
    size_t current_token_index;

    // Get Next Token
    Token get_next_token();

    // Parse from Top to Bottom
    void parseProgram();
    void parseStatement();
    void parseExpression();
    void parseTerm();
    void parseFactor();

    // Helper
    bool match_token(Token source, Token expected, bool strict_matching);
};


