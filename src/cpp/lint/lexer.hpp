#pragma once

#include "token.hpp"

enum State {
    START,
    IN_OPERATOR,
    IN_NUMBER,
    IN_IDENTIFIER,
    FINISH
};

bool __is_reserved(std::string identifier);
bool __match_or(std::string chars_to_match, char curr_ch, bool is_null_check, bool check_new_line = true);

Token getNextToken(std::string input, int &index, int &src_line);