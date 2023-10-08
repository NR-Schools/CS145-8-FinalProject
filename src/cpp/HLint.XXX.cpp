#include <algorithm>
#include <iostream>
#include <fstream>
#include <string>
#include <vector>

#include "lint/lexer.hpp"
#include "lint/parser.hpp"
#include "interpreter/interpreter.hpp"


void help()
{
    printf("Please pass the source file path as the only argument\n");
}

std::string read_file(const char *filename)
{
    std::string lines, line;

    std::ifstream rfile;
    rfile.open(filename);
    while (std::getline(rfile, line))
    {
        lines += line + '\n';
    }
    rfile.close();

    return lines;
}

void write_file(const char *filename, std::string contents) {
    std::ofstream out_file;
    out_file.open(filename);
    out_file << contents << '\n';
    out_file.close();
}

std::vector<Token> lexer(std::string lines)
{
    std::vector<Token> tokens;
    Token token;
    int index = 0;

    // Used for detailed response
    int src_line = 1;

    while ((token = getNextToken(lines, index, src_line)).type != TOKEN_END)
    {
        if (token.type == TOKEN_INVALID)
        {
            //std::cerr << "ERROR" << '\n';
            exit(1);
        }

        tokens.push_back(token);
    }

    return tokens;
}


int main(int argc, char *argv[])
{
    // Show help if no arguments/too many args passed
    if (argc <= 1 || argc > 2)
    {
        help();
        return 0;
    }

    // Start Reading File
    std::string rfile_content = read_file(argv[1]);

    // Output to NOSPACES.TXT
    rfile_content.erase(remove(rfile_content.begin(), rfile_content.end(), ' '), rfile_content.end());
    write_file("NOSPACES.TXT", rfile_content);

    // Start Scanning (Lexer)
    std::vector<Token> tokens = lexer(rfile_content);

    // Output to RES_SYM.TXT
    std::string res_words = "Reserved Words: ";
    std::string symbols = "Symbols: ";
    for(unsigned int i = 0; i < tokens.size(); i++) {
        // Reserved Words
        if (tokens[i].type == TokenType::TOKEN_RESERVED) {
            res_words += tokens[i].lexeme;

            if (i+1 < tokens.size()) res_words += " ";
        }

        // Special Symbols
        if (tokens[i].type == TokenType::TOKEN_OPERATOR || tokens[i].type == TokenType::TOKEN_SEPARATOR) {
            symbols += tokens[i].lexeme;

            if (i+1 < tokens.size()) symbols += " ";
        }
    }
    write_file("RES_SYM.TXT", res_words + '\n' + symbols);

    // Start Parsing (Parser)
    Parser parser(tokens, false, false);
    ASTNode root = parser.parse();
    
    if (parser.err_flag) {
        std::cout << "ERROR" << std::endl;
        exit(1);
    }
    
    std::cout << "NO ERROR(S) FOUND" << std::endl;

    return 0;
}