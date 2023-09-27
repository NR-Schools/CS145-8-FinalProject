#include <stdio.h>
#include <stdlib.h>

#include "lint/helper_lib/dyn_str.h"
#include "lint/helper_lib/token_arr.h"
#include "lint/lexer.h"

#define MAX_LINE_LENGTH 1000

void read_file(const char *filename, dyn_str *dynamic_str, bool *err_flag)
{
    FILE *f_ptr;
    f_ptr = fopen(filename, "r");
    char content[MAX_LINE_LENGTH];

    if (f_ptr == NULL)
    {
        printf("Error: Unable to read %s", filename);
        *err_flag = true;
    }

    while (fgets(content, MAX_LINE_LENGTH, f_ptr))
    {
        dyn_str_append(dynamic_str, content);
    }

    fclose(f_ptr);
}

void lexer(char *content, TokenArray *tok_arr, bool *err_flag)
{
    Token token;
    int index = 0;

    // Used for detailed response
    int src_line = 1;

    while ((token = getNextToken(content, &index, &src_line)).type != TOKEN_END)
    {
        //printf("Token: %s, Type: %d\n", token.lexeme, token.type);
        if (token.type == TOKEN_INVALID)
        {
            printf("Invalid token: %s at line %d \n", token.lexeme, src_line);
            *err_flag = true;
            break;
        }

        token_append_arr(tok_arr, token);
    }
}

void help()
{
    printf("Please pass the source file path as the only argument\n");
    printf("HLint (source file path)\n");
}

int main(int argc, char *argv[])
{
    // Show help if no arguments/too many args passed
    if (argc <= 1 || argc > 2)
    {
        help();
        return 0;
    }

    bool *err_flag = false;

    // Start Reading File
    dyn_str *dynamic_str = dyn_str_create();
    read_file(argv[argc-1], dynamic_str, err_flag);

    if (err_flag) {
        printf("Error Reading Source File!\n");
        return 1;
    }

    // Start Scanning (Lexer)
    TokenArray *token_arr = token_create_arr();
    lexer(dynamic_str->data, token_arr, err_flag);

    if (err_flag) {
        printf("Error in getting Tokens!\n");
        return 1;
    }
    
    // Start Parsing (Parser)
    for (size_t i = 0; i < token_arr->size; i++) {
        Token token = token_arr->tokens[i];
        printf("Token: %s, Type: %d\n", token.lexeme, token.type);
    }

    // Free memories
    dyn_str_free(dynamic_str);
    token_free_arr(token_arr);

    return 0;
}