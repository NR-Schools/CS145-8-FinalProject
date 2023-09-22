#include <stdio.h>

void help() {
    printf("Please pass the source file path as the only argument");
    printf("HLint (source file path)");
}

void lexer() {
    //
}

void parser() {
    //
}

int main(int argc, char *argv[]) {

    // Show help if no arguments passed
    if (argc == 1) {
        help();
        return 0;
    }

    // Run lexer and parser
    lexer();
    parser();

    return 0;
}