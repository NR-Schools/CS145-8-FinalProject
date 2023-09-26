#pragma once

#include "../types.h"

#include <stdio.h>
#include <stdlib.h>


typedef struct {
    Token* tokens;
    size_t size;
    size_t capacity;
} TokenArray;

TokenArray* token_create_arr();
void token_append_arr(TokenArray* arr, Token token);
void token_free_arr(TokenArray* arr);