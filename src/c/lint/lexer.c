#include "lexer.h"

bool __match_or(const char *chars_to_match, char curr_ch, bool is_null_check)
{
    if (is_null_check && curr_ch == '\0')
        return true;
    for (int i = 0; i < length(chars_to_match); i++)
    {
        if (chars_to_match[i] == curr_ch)
            return true;
    }
    return false;
}

bool __is_reserved(char *identifier, int lexeme_len)
{
    char *reserved[4] = {"integer", "double", "if", "output"};
    bool _flag = false;

    for (int i = 0; i < length(reserved); i++)
    {
        // Reset flag
        _flag = false;

        // Compare First Sizes
        if (strlen(reserved[i]) != lexeme_len)
            continue;

        for (int j = 0; j < strlen(reserved[i]); j++)
        {
            if (reserved[i][j] != identifier[j])
            {
                _flag = true;
                break;
            }
        }

        if (!_flag)
            return true;
    }

    return false;
}

Token getNextToken(char *input, int *index, int *src_line)
{
    Token token;
    enum State current_state = STATE_START;
    int lexeme_ind = 0;

    // Number and String end pattern
    const char *NUMBER_ENDS = " ();+-<>=!";
    const char *IDENT_ENDS = " ();:+-<>=!";

    // For numbers
    bool has_used_dot = false;

    while (current_state != STATE_FINISH)
    {
        char curr_char = input[*index];

        // Skip all spaces
        while (curr_char == ' ' || curr_char == '\n')
        {
            if (curr_char == '\n')
                (*src_line)++;

            (*index)++;
            curr_char = input[*index];
        }
        char next_char = input[*index + 1];

        // Check if End of Token
        if (curr_char == '\0')
        {
            token.type = TOKEN_END;
            current_state = STATE_FINISH;
        }

        switch (current_state)
        {
        case STATE_START:
        {
            // Check all purely single character tokens only
            if (curr_char == '(' || curr_char == ')' || curr_char == ';' || curr_char == '+' || curr_char == '-')
            {
                if (curr_char == '(' || curr_char == ')' || curr_char == ';')
                    token.type = TOKEN_SEPARATOR;
                else if (curr_char == '+' || curr_char == '-')
                    token.type = TOKEN_OPERATOR;

                token.lexeme[lexeme_ind++] = curr_char;
                current_state = STATE_FINISH;
            }

            // Symbols that can be one or more tokens
            else if (curr_char == ':')
            {
                token.type = TOKEN_SEPARATOR;
                token.lexeme[lexeme_ind++] = curr_char;

                if (next_char == '=')
                {
                    token.lexeme[lexeme_ind++] = next_char;
                    token.type = TOKEN_OPERATOR;

                    // Go to next character (due to lookahead)
                    (*index)++;
                }

                current_state = STATE_FINISH;
            }
            else if (curr_char == '<')
            {
                token.type = TOKEN_OPERATOR;
                token.lexeme[lexeme_ind++] = curr_char;

                if (next_char == '<' || next_char == '=')
                {
                    token.lexeme[lexeme_ind++] = next_char;

                    // Go to next character (due to lookahead)
                    (*index)++;
                }

                current_state = STATE_FINISH;
            }
            else if (curr_char == '>')
            {
                token.type = TOKEN_OPERATOR;
                token.lexeme[lexeme_ind++] = curr_char;

                if (next_char == '=')
                {
                    token.lexeme[lexeme_ind++] = next_char;

                    // Go to next character (due to lookahead)
                    (*index)++;
                }

                current_state = STATE_FINISH;
            }

            // Required two characters
            else if ((curr_char == '!' || curr_char == '=') && next_char == '=')
            {
                token.type = TOKEN_OPERATOR;
                token.lexeme[lexeme_ind++] = curr_char;
                token.lexeme[lexeme_ind++] = next_char;

                // Go to next character (due to lookahead)
                (*index)++;
                
                current_state = STATE_FINISH;
            }

            // Check for numbers
            else if (isdigit(curr_char))
            {
                token.type = TOKEN_INTEGER;
                token.lexeme[lexeme_ind++] = curr_char;
                current_state = STATE_IN_NUMBER;

                // Do via Lookahead
                // Check for symbols that can cut off numbers
                if (__match_or(NUMBER_ENDS, next_char, true))
                {
                    current_state = STATE_FINISH;
                }
            }

            // Check for all possible identifiers
            else if ((curr_char >= 'A' && curr_char <= 'Z') || (curr_char >= 'a' && curr_char <= 'z'))
            {
                token.type = TOKEN_IDENTIFIER;
                token.lexeme[lexeme_ind++] = curr_char;
                current_state = STATE_IN_IDENTIFIER;

                // Do via Lookahead
                // Check for symbols that can cut off identifier
                if (__match_or(IDENT_ENDS, next_char, true))
                {
                    current_state = STATE_FINISH;
                }
            }

            // Invalid Symbols
            else
            {
                printf("%c \n", curr_char);
                token.type = TOKEN_INVALID;
                token.lexeme[lexeme_ind++] = curr_char;
                current_state = STATE_FINISH;
                break;
            }
        }
        break;
        case STATE_IN_NUMBER:
        {
            // Check for number
            if (isdigit(curr_char))
            {
                token.lexeme[lexeme_ind++] = curr_char;
            }

            // Check for single-use dot
            else if (curr_char == '.' && !has_used_dot)
            {
                token.type = TOKEN_DECIMAL;
                token.lexeme[lexeme_ind++] = curr_char;
                has_used_dot = true;
            }

            // Invalid Characters
            else
            {
                token.type = TOKEN_INVALID;
                token.lexeme[lexeme_ind++] = curr_char;
                current_state = STATE_FINISH;
                break;
            }

            // Do via Lookahead
            // Check for symbols that can cut off numbers
            if (__match_or(NUMBER_ENDS, next_char, true))
            {
                current_state = STATE_FINISH;
            }
        }
        break;
        case STATE_IN_IDENTIFIER:
        {
            if ((curr_char >= 'A' && curr_char <= 'Z') || (curr_char >= 'a' && curr_char <= 'z'))
            {
                token.lexeme[lexeme_ind++] = curr_char;
            }

            // Invalid Characters
            else
            {
                token.type = TOKEN_INVALID;
                token.lexeme[lexeme_ind++] = curr_char;
                current_state = STATE_FINISH;
                break;
            }

            // Do via Lookahead
            // Check for symbols that can cut off identifier
            if (__match_or(IDENT_ENDS, next_char, true))
            {
                if (__is_reserved(token.lexeme, lexeme_ind))
                    token.type = TOKEN_RESERVED;
                else
                    token.type = TOKEN_IDENTIFIER;

                current_state = STATE_FINISH;
            }
        }
        break;
        case STATE_FINISH:
            break;
        }

        // Go to next character
        (*index)++;
    }

    token.lexeme[lexeme_ind++] = '\0';

    return token;
}