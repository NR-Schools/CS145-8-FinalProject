#include "dyn_str.h"


dyn_str* dyn_str_create() {
    dyn_str* str = (dyn_str*)malloc(sizeof(dyn_str));
    if (str == NULL) {
        perror("Err: Unable to perform memory allocation");
        exit(EXIT_FAILURE);
    }

    str->data = NULL;
    str->length = 0;
    str->capacity = 0;

    return str;
}

void dyn_str_append(dyn_str* str, const char* append_str) {
    size_t append_len = strlen(append_str);

    if (str->length + append_len + 1 > str->capacity) {
        size_t new_capacity = str->capacity + (append_len + 1);
        char* new_data = (char*)realloc(str->data, new_capacity);

        if (new_data == NULL) {
            perror("Err: Unable to perform memory reallocation");
            exit(EXIT_FAILURE);
        }

        memset(new_data + str->length, 0, append_len + 1);

        str->data = new_data;
        str->capacity = new_capacity;
    }

    strcat(str->data, append_str);
    str->length += append_len;
}

void dyn_str_free(dyn_str* str) {
    free(str->data);
    free(str);
}
