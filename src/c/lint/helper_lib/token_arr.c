#include "token_arr.h"

TokenArray* token_create_arr() {
    TokenArray* arr = (TokenArray*)malloc(sizeof(TokenArray));
    if (arr == NULL) {
        perror("Err: Unable to perform memory allocation");
        exit(EXIT_FAILURE);
    }

    arr->tokens = NULL;
    arr->size = 0;
    arr->capacity = 0;
    return arr;
}
#include "token_arr.h"

void token_append_arr(TokenArray* arr, Token token) {
    if (arr->size == arr->capacity) {
        arr->capacity = (arr->capacity == 0) ? 1 : arr->capacity * 2;
        arr->tokens = (Token*)realloc(arr->tokens, sizeof(Token) * arr->capacity);

        if (arr->tokens == NULL) {
            perror("Err: Unable to perform memory reallocation");
            exit(EXIT_FAILURE);
        }
    }

    // Append the token
    arr->tokens[arr->size] = token;
    arr->size++;
}

void token_free_arr(TokenArray* arr) {
    free(arr->tokens);
    free(arr);
}
