#pragma once

#include <vector>
#include <string>
#include <sstream>

#include "token.hpp"
#include "ast.hpp"

class Parser
{
public:
    Parser(const std::vector<Token>& tokens);
    ASTNode parse();

private:
    std::vector<Token> tokens;
    size_t current_token_index;

    // Parse from Top to Bottom
    ASTNode parseProgram();
    ASTNode parseStatement();
    ASTNode parseExpression();
    ASTNode parseTerm();
    ASTNode parseFactor();

    // Helper
    Token get_curr_token();
    bool peek_and_compare_future_token(TokenType target_type, std::vector<std::string> target_lexemes, bool strict_matching, int next=1);
    void advance_to_next_token();
    bool match_token(TokenType target_type, std::vector<std::string> target_lexemes, bool strict_matching);

    void unexpected_token_error(std::string expected_token);
    void unable_to_parse_error(std::string level);
};


