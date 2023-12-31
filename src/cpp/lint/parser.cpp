#include <iostream>

#include "parser.hpp"

Parser::Parser(const std::vector<Token> &tokens, bool _enable_print, bool _allow_short_circuit)
{
    this->tokens = tokens;
    this->current_token_index = 0;
    this->err_flag = false;
    this->enable_print = _enable_print;
    this->allow_short_circuit = _allow_short_circuit;
}

ASTNode Parser::parse()
{
    if (this->tokens.size() == 0 && this->enable_print)
    {
        std::cout << "No Tokens Passed" << std::endl;
        if (this->allow_short_circuit)
            exit(1);
        this->err_flag = true;
        return ASTNode();
    }
    return parseProgram();
}

ASTNode Parser::parseProgram()
{
    ASTNode rootNode;
    rootNode.set_node_type(ASTNodeType::PROGRAM);
    while (this->current_token_index + 1 < this->tokens.size())
    {
        ASTNode stmt = this->parseStatement();
        rootNode.add_child_node(stmt);

        // Check err
        if (this->err_flag)
        {
            return ASTNode();
        }

        // Move to next token
        this->advance_to_next_token();
    }

    return rootNode;
}

ASTNode Parser::parseStatement()
{
    if (this->err_flag)
    {
        return ASTNode();
    }

    ASTNode statementNode;

    // Check if this is an IfNode
    if (this->match_token(TokenType::TOKEN_RESERVED, {"if"}, true))
    {
        statementNode.set_node_type(ASTNodeType::IF);
        statementNode.set_value("if");

        this->advance_to_next_token();
        if (!this->match_token(TokenType::TOKEN_SEPARATOR, {"("}, true))
        {
            // Error
            this->unexpected_token_error("(");
        }

        this->advance_to_next_token();
        ASTNode expression_node = this->parseExpression();
        statementNode.add_child_node(expression_node);

        this->advance_to_next_token();
        if (!this->match_token(TokenType::TOKEN_SEPARATOR, {")"}, true))
        {
            // Error
            this->unexpected_token_error(")");
        }

        this->advance_to_next_token();
        ASTNode statement_node = this->parseStatement();
        statementNode.add_child_node(statement_node);

        return statementNode;
    }

    // Check if this is an OutputNode
    if (this->match_token(TokenType::TOKEN_RESERVED, {"output"}, true))
    {
        statementNode.set_node_type(ASTNodeType::OUTPUT);
        statementNode.set_value("<<");

        this->advance_to_next_token();
        if (!this->match_token(TokenType::TOKEN_OPERATOR, {"<<"}, true))
        {
            // Error
            this->unexpected_token_error("<<");
        }

        this->advance_to_next_token();
        ASTNode expression_node = this->parseExpression();
        statementNode.add_child_node(expression_node);

        this->advance_to_next_token();
        if (!this->match_token(TokenType::TOKEN_SEPARATOR, {";"}, true))
        {
            // Error
            this->unexpected_token_error(";");
        }

        return statementNode;
    }

    // Check if Identifier
    if (this->match_token(TokenType::TOKEN_IDENTIFIER, {}, false))
    {
        Token identifier = this->get_curr_token();
        ASTNode identifier_node;
        identifier_node.set_node_type(ASTNodeType::ATOM);
        identifier_node.set_value(identifier.lexeme);
        statementNode.add_child_node(identifier_node);

        this->advance_to_next_token();

        // Check if Declaration
        if (this->match_token(TokenType::TOKEN_OPERATOR, {":"}, true))
        {
            statementNode.set_node_type(ASTNodeType::DECLARATION);
            statementNode.set_value(":");

            this->advance_to_next_token();
            if (!this->match_token(TokenType::TOKEN_RESERVED, {"integer", "double"}, true))
            {
                // Error
                this->unexpected_token_error("integer or double");
            }

            ASTNode identifier_data_type;
            identifier_data_type.set_node_type(ASTNodeType::ATOM);
            identifier_data_type.set_value(this->get_curr_token().lexeme);
            statementNode.add_child_node(identifier_data_type);

            this->advance_to_next_token();
            if (!this->match_token(TokenType::TOKEN_SEPARATOR, {";"}, true))
            {
                // Error
                this->unexpected_token_error(";");
            }

            return statementNode;
        }

        // Check if Assignment
        if (this->match_token(TokenType::TOKEN_OPERATOR, {":="}, true))
        {
            statementNode.set_node_type(ASTNodeType::ASSIGNMENT);
            statementNode.set_value(":=");

            // Check if this is for expression or factor
            this->advance_to_next_token();
            ASTNode expression_node = this->parseExpression();
            statementNode.add_child_node(expression_node);

            this->advance_to_next_token();
            if (!this->match_token(TokenType::TOKEN_SEPARATOR, {";"}, true))
            {
                // Error
                this->unexpected_token_error(";");
            }

            return statementNode;
        }
    }

    // Error
    this->unable_to_parse_error("statement");

    return ASTNode();
}

ASTNode Parser::parseExpression()
{
    if (this->err_flag)
    {
        return ASTNode();
    }

    ASTNode expressionNode;
    expressionNode.set_node_type(ASTNodeType::NONE);

    ASTNode initial_term_node = this->parseTerm();
    expressionNode.add_child_node(initial_term_node);

    // Check if next token is a separator
    if (this->peek_and_compare_future_token(TokenType::TOKEN_SEPARATOR, {}, false))
    {
        return expressionNode;
    }

    expressionNode.set_node_type(ASTNodeType::BINARY_OP);

    // Check if Comparison
    if (this->peek_and_compare_future_token(TokenType::TOKEN_OPERATOR, {"!=", "==", "<=", ">=", "<", ">"}, true))
    {
        // Push back operator
        this->advance_to_next_token();
        expressionNode.set_value(this->get_curr_token().lexeme);

        // Get next term
        this->advance_to_next_token();
        ASTNode term_node = this->parseTerm();
        expressionNode.add_child_node(term_node);

        return expressionNode;
    }

    // Check if Arithmetic (+, -)
    if (this->peek_and_compare_future_token(TokenType::TOKEN_OPERATOR, {"+", "-"}, true))
    {
        while (this->peek_and_compare_future_token(TokenType::TOKEN_OPERATOR, {"+", "-"}, true))
        {
            ASTNode _node;

            // Push back operator
            this->advance_to_next_token();
            _node.set_value(this->get_curr_token().lexeme);

            // Get next term
            this->advance_to_next_token();
            ASTNode term_node = this->parseTerm();

            // Push initial_term_node and term_node under _node and replace initial_term_node with _node
            _node.add_child_node(initial_term_node);
            _node.add_child_node(term_node);
            _node.set_node_type(ASTNodeType::BINARY_OP);
            initial_term_node = _node;
        }

        return initial_term_node;
    }

    // Error
    this->unable_to_parse_error("expression");

    return ASTNode();
}

ASTNode Parser::parseTerm()
{
    if (this->err_flag)
    {
        return ASTNode();
    }

    ASTNode termNode;

    ASTNode initial_factor_node = this->parseFactor();
    termNode.set_node_type(ASTNodeType::NONE);
    termNode.add_child_node(initial_factor_node);

    // Check if Arithmetric (*, /)
    if (this->peek_and_compare_future_token(TokenType::TOKEN_OPERATOR, {"*", "/"}, true))
    {
        while (this->peek_and_compare_future_token(TokenType::TOKEN_OPERATOR, {"*", "/"}, true))
        {
            ASTNode _node;

            // Push back operator
            this->advance_to_next_token();
            _node.set_value(this->get_curr_token().lexeme);

            // Get next term
            this->advance_to_next_token();
            ASTNode term_node = this->parseTerm();

            // Push initial_term_node and term_node under _node and replace initial_term_node with _node
            _node.add_child_node(initial_factor_node);
            _node.add_child_node(term_node);
            _node.set_node_type(ASTNodeType::BINARY_OP);
            initial_factor_node = _node;
        }

        return initial_factor_node;
    }

    return termNode;
}

ASTNode Parser::parseFactor()
{
    if (this->err_flag)
    {
        return ASTNode();
    }

    ASTNode factorNode;
    if (this->match_token(TokenType::TOKEN_IDENTIFIER, {}, false) ||
        this->match_token(TokenType::TOKEN_INTEGER, {}, false) ||
        this->match_token(TokenType::TOKEN_DECIMAL, {}, false))
    {
        factorNode.set_node_type(ASTNodeType::ATOM);
        factorNode.set_value(this->get_curr_token().lexeme);
        return factorNode;
    }

    // Error
    this->unable_to_parse_error("factor");

    return ASTNode();
}

Token Parser::get_curr_token()
{
    return this->tokens[this->current_token_index];
}

bool Parser::peek_and_compare_future_token(TokenType target_type, std::vector<std::string> target_lexemes, bool strict_matching, int next)
{
    // Get Token
    int peek_index = this->current_token_index + next;
    if (peek_index >= this->tokens.size())
    {
        return false;
    }
    Token future_token = this->tokens[peek_index];

    // Compare Token
    if (target_type != future_token.type)
    {
        return false;
    }

    if (strict_matching)
    {
        for (std::string target_lexeme : target_lexemes)
        {
            if (target_lexeme.compare(future_token.lexeme) == 0)
                return true;
        }
        return false;
    }
    return true;
}

void Parser::advance_to_next_token()
{
    if (this->current_token_index + 1 < this->tokens.size())
    {
        this->current_token_index++;
    }
}

bool Parser::match_token(TokenType target_type, std::vector<std::string> target_lexemes, bool strict_matching)
{
    Token current_token = this->get_curr_token();
    if (target_type != current_token.type)
    {
        return false;
    }

    if (strict_matching)
    {
        for (std::string target_lexeme : target_lexemes)
        {
            if (target_lexeme.compare(current_token.lexeme) == 0)
                return true;
        }
        return false;
    }
    return true;
}

void Parser::unexpected_token_error(std::string expected_token)
{
    if (this->enable_print)
    {
        std::string message = "Syntax Error: Expecting \"" + expected_token + "\" but was \"" + this->get_curr_token().lexeme + "\"";
        std::cout << message << std::endl;
    }
    if (this->allow_short_circuit)
        exit(1);
    this->err_flag = true;
}

void Parser::unable_to_parse_error(std::string level)
{
    if (this->enable_print)
    {
        std::string message = "Syntax Error: Unable to parse " + level + ", token found was \"" + this->get_curr_token().lexeme + "\"";
        std::cout << message << std::endl;
    }
    if (this->allow_short_circuit)
        exit(1);
    this->err_flag = true;
}
