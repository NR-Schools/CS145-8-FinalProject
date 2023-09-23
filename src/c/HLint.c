#include <stdio.h>
#include "lint/lexer.h"

void help() {
    printf("Please pass the source file path as the only argument \n");
    printf("HLint (source file path)");
}

#define PLAYGROUND

int main(int argc, char *argv[]) {

#ifdef PLAYGROUND
    char input[200]; // at most 200 bytes
    scanf_s("%s", &input);

    int index = 0;
    struct Token token;
    
    while ((token = getNextToken(input, &index)).type != TOKEN_END) {
        printf("Token: %s, Type: %d\n", token.lexeme, token.type);
        if (token.type == TOKEN_INVALID) {
            printf("Invalid token: %s at col %d \n", token.lexeme, index);
            break;
        }
    }
#endif

    // Show help if no arguments passed
    if (argc == 1) {
        help();
        return 0;
    }

    return 0;
}