#include <iostream>

#include "parser.hpp"

Parser::Parser(const std::vector<Token>& tokens)
{
    this->tokens = tokens;
    this->current_token_index = 0;
}

void Parser::parse()
{
}

Token Parser::get_next_token()
{
    //
}

void Parser::parseProgram()
{
    //
}

void Parser::parseStatement()
{
    //
}

void Parser::parseExpression()
{
    //
}

void Parser::parseTerm()
{
}

void Parser::parseFactor()
{
}

bool Parser::match_token(Token source, Token expected, bool strict_matching)
{
    if (source.type != expected.type) return false;
    if (strict_matching) return source.lexeme.compare(expected.lexeme) == 0 ? true : false;
}
