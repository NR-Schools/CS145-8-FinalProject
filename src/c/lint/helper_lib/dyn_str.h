#pragma once

#include <stdio.h>
#include <stdlib.h>
#include <string.h>


typedef struct {
    char* data;      // Pointer to the character data
    size_t length;   // Current length of the string
    size_t capacity; // Current capacity of the allocated memory
} dyn_str;


dyn_str* dyn_str_create();
void dyn_str_append(dyn_str* str, const char* append_str);
void dyn_str_free(dyn_str* str);
