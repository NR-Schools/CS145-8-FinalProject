#include "lexer.hpp"

bool __match_or(std::string chars_to_match, char src_ch, bool is_null_check, bool check_new_line)
{
    if (check_new_line && src_ch == '\n')
        return true;

    if (is_null_check && src_ch == '\0')
        return true;

    for (char ch_match : chars_to_match)
    {
        if (ch_match == src_ch)
            return true;
    }

    return false;
}

bool __is_reserved(std::string identifier)
{
    std::string reserved_arr[] = {"integer", "double", "if", "output", "fn", "return"};

    for (std::string reserved_str : reserved_arr)
    {
        if (identifier.compare(reserved_str) == 0)
            return true;
    }

    return false;
}

Token getNextToken(std::string input, int &index, int &src_line)
{
    Token token = Token();
    State current_state = State::START;

    // Number and String end pattern
    const char *NUMBER_ENDS = " (){},;+-*/<>=!";
    const char *IDENT_ENDS = " (){},;:+-*/<>=!";

    // For numbers
    bool has_used_dot = false;

    while (current_state != State::FINISH)
    {
        char curr_char = input[index];

        // Skip all spaces
        while (curr_char == ' ' || curr_char == '\n')
        {
            if (curr_char == '\n')
                src_line++;

            index++;
            curr_char = input[index];
        }

        // Get Next Character
        char next_char = input[index + 1];

        // Check if End of Token
        if (curr_char == '\0')
        {
            token.type = TOKEN_END;
            current_state = State::FINISH;
        }

        switch (current_state)
        {
        case State::START:
        {
            // Check all purely single character tokens only
            if (curr_char == '{' || curr_char == '}' ||
                curr_char == '(' || curr_char == ')' || curr_char == ';' || curr_char == ',' ||
                curr_char == '+' || curr_char == '-' || curr_char == '*' || curr_char == '/')
            {
                if (curr_char == '{' || curr_char == '}' || curr_char == '(' || curr_char == ')' || curr_char == ';' || curr_char == ',')
                    token.type = TOKEN_SEPARATOR;
                else if (curr_char == '+' || curr_char == '-' || curr_char == '*' || curr_char == '/')
                    token.type = TOKEN_OPERATOR;

                token.lexeme += curr_char;
                current_state = State::FINISH;
            }

            // Symbols that can be one or more tokens
            else if (curr_char == ':')
            {
                token.type = TOKEN_OPERATOR;
                token.lexeme += curr_char;

                if (next_char == '=')
                {
                    token.lexeme += next_char;

                    // Go to next character (due to lookahead)
                    index++;
                }

                current_state = State::FINISH;
            }
            else if (curr_char == '<')
            {
                token.type = TOKEN_OPERATOR;
                token.lexeme += curr_char;

                if (next_char == '<' || next_char == '=')
                {
                    token.lexeme += next_char;

                    // Go to next character (due to lookahead)
                    index++;
                }

                current_state = State::FINISH;
            }
            else if (curr_char == '>')
            {
                token.type = TOKEN_OPERATOR;
                token.lexeme += curr_char;

                if (next_char == '=')
                {
                    token.lexeme += next_char;

                    // Go to next character (due to lookahead)
                    index++;
                }

                current_state = State::FINISH;
            }

            // Required two characters
            else if (curr_char == '!')
            {
                token.lexeme += curr_char;
                token.type = TOKEN_INVALID;

                if (next_char == '=')
                {
                    token.lexeme += next_char;
                    token.type = TOKEN_OPERATOR;
                    // Go to next character (due to lookahead)
                    index++;
                }
                
                current_state = State::FINISH;
            }
            else if (curr_char == '=')
            {
                token.lexeme += curr_char;
                token.type = TOKEN_INVALID;

                if (next_char == '=')
                {
                    token.lexeme += next_char;
                    token.type = TOKEN_OPERATOR;
                    // Go to next character (due to lookahead)
                    index++;
                }
                else if (next_char == '>')
                {
                    token.lexeme += next_char;
                    token.type = TOKEN_SEPARATOR;
                    // Go to next character (due to lookahead)
                    index++;
                }
                
                current_state = State::FINISH;
            }

            // Check for numbers
            else if (isdigit(curr_char))
            {
                token.type = TOKEN_INTEGER;
                token.lexeme += curr_char;
                current_state = State::IN_NUMBER;

                // Do via Lookahead
                // Check for symbols that can cut off numbers
                if (__match_or(NUMBER_ENDS, next_char, true))
                {
                    current_state = State::FINISH;
                }
            }

            // Check for all possible identifiers
            else if ((curr_char >= 'A' && curr_char <= 'Z') || (curr_char >= 'a' && curr_char <= 'z'))
            {
                token.type = TOKEN_IDENTIFIER;
                token.lexeme += curr_char;
                current_state = State::IN_IDENTIFIER;

                // Do via Lookahead
                // Check for symbols that can cut off identifier
                if (__match_or(IDENT_ENDS, next_char, true))
                {
                    current_state = State::FINISH;
                }
            }

            // Invalid Symbols
            else
            {
                printf("%c \n", curr_char);
                token.type = TOKEN_INVALID;
                token.lexeme += curr_char;
                current_state = State::FINISH;
                break;
            }
        }
        break;
        case State::IN_NUMBER:
        {
            // Check for number
            if (isdigit(curr_char))
            {
                token.lexeme += curr_char;
            }

            // Check for single-use dot
            else if (curr_char == '.' && !has_used_dot)
            {
                token.type = TOKEN_DECIMAL;
                token.lexeme += curr_char;
                has_used_dot = true;
            }

            // Invalid Characters
            else
            {
                token.type = TOKEN_INVALID;
                token.lexeme += curr_char;
                current_state = State::FINISH;
                break;
            }

            // Do via Lookahead
            // Check for symbols that can cut off numbers
            if (__match_or(NUMBER_ENDS, next_char, true))
            {
                current_state = State::FINISH;
            }
        }
        break;
        case State::IN_IDENTIFIER:
        {
            if ((curr_char >= 'A' && curr_char <= 'Z') || (curr_char >= 'a' && curr_char <= 'z'))
            {
                token.lexeme += curr_char;
            }

            // Invalid Characters
            else
            {
                token.type = TOKEN_INVALID;
                token.lexeme += curr_char;
                current_state = State::FINISH;
                break;
            }

            // Do via Lookahead
            // Check for symbols that can cut off identifier
            if (__match_or(IDENT_ENDS, next_char, true))
            {
                if (__is_reserved(token.lexeme))
                    token.type = TOKEN_RESERVED;
                else
                    token.type = TOKEN_IDENTIFIER;

                current_state = State::FINISH;
            }
        }
        break;
        case State::FINISH:
            break;
        }

        // Go to next character
        index++;
    }

    return token;
}