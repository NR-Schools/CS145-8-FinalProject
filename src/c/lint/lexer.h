#pragma once

#include "types.h"
#include <stdio.h>
#include <stdbool.h>
#include <ctype.h>
#include <string.h>

#define length(array) ((sizeof(array)) / (sizeof(array[0])))

enum State {
    STATE_START,
    STATE_IN_OPERATOR,
    STATE_IN_NUMBER,
    STATE_IN_IDENTIFIER,
    STATE_FINISH
};

bool is_reserved(char* identifier, int lexeme_len);
bool match_next_or(const char* chars_to_match, char curr_ch, bool is_null_check);
struct Token getNextToken(char *input, int *index);