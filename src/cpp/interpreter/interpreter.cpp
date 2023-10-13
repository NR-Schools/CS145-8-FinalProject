#include "interpreter.hpp"

Interpreter::Interpreter(ASTNode root)
{
    this->root = root;
}

Interpreter::~Interpreter()
{
}

void Interpreter::interpret()
{
    // Execute statements under root node
    for (ASTNode statement_node : this->root.get_child_nodes())
    {
        this->interpret_statement(statement_node);
    }
}

std::string Interpreter::runtime_casting(ExprValType type, std::string value)
{
    std::string new_value = value;

    if (type == ExprValType::INTEGER)
    {
        new_value = "";
        for (char c : value)
        {
            if (c == '.')
                break;
            new_value += c;
        }
    }

    return new_value;
}

bool Interpreter::is_valid_value(std::string value_str)
{
    std::istringstream stream(value_str);
    double value;
    return (stream >> std::noskipws >> value) && (stream.eof() || stream.peek() == '\n');
}

template <typename T, typename>
T Interpreter::perform_calculation(std::string left, std::string right, std::string op)
{
    T left_val = (T)std::stod(left);
    T right_val = (T)std::stod(right);
    T final_val;

    if (op.compare("+") == 0)
        final_val = left_val + right_val;
    else if (op.compare("-") == 0)
        final_val = left_val - right_val;
    else if (op.compare("*") == 0)
        final_val = left_val * right_val;
    else if (op.compare("/") == 0)
        final_val = left_val / right_val;

    else if (op.compare("==") == 0)
        final_val = left_val == right_val;
    else if (op.compare("!=") == 0)
        final_val = left_val != right_val;
    else if (op.compare(">") == 0)
        final_val = left_val > right_val;
    else if (op.compare(">=") == 0)
        final_val = left_val >= right_val;
    else if (op.compare("<") == 0)
        final_val = left_val < right_val;
    else if (op.compare("<=") == 0)
        final_val = left_val <= right_val;
    else
        this->runtime_error("Unsupported operation: update this interpreter for additional operation support");

    return final_val;
}

template <typename T, typename>
T Interpreter::calculated_expression_formatter(T val)
{
    // Check if double
    // via casting to string
    ExprValType val_type = this->assert_type_from_value(std::to_string(val));

    if (val_type == ExprValType::DOUBLE)
    {
        double value = (int)(val * 100 + .5);
        return (double)value / 100;
    }
    return val;
}

std::string Interpreter::remove_trailing_zeroes(ExprVal expr_value)
{
    // Only perform if value is double
    if (expr_value.type == ExprValType::DOUBLE)
    {
        std::string expr_val_str = expr_value.value + '0';
        int end = 0;
        for (int i = 0; i < expr_val_str.length(); i++)
        {
            if (expr_val_str[i] != '0')
                end = i;
        }
        return expr_val_str.substr(0, end+1);
    }
    return expr_value.value;
}

std::string Interpreter::get_value_if_variable(std::string unknown_atom)
{
    std::string value = unknown_atom;

    // If variable -> return var value, else, just return value
    if (this->var_map.find(value) != this->var_map.end())
    {
        value = this->var_map[unknown_atom].value;
    }

    // Check if valid value
    if (!this->is_valid_value(value))
    {
        // Runtime Error
        this->runtime_error(
            "Unexpected value encountered on runtime: \"" + value + "\"");
    }

    return value;
}

ExprValType Interpreter::assert_type_from_value(std::string unknown_value)
{
    // If a dot exists
    std::size_t found = unknown_value.find('.');
    if (found != std::string::npos)
        return ExprValType::DOUBLE;
    return ExprValType::INTEGER;
}

void Interpreter::interpret_statement(ASTNode statementNode)
{
    // Get operation for statement node
    switch (statementNode.get_node_type())
    {
    case ASTNodeType::IF:
    {
        // Get Expression
        ExprVal if_expr = this->interpret_expression(statementNode.get_child_nodes()[0]);

        // Get Next Statement
        ASTNode if_stmt_node = statementNode.get_child_nodes()[1];

        // Check value: execute statement if expr > 0
        if (std::stod(if_expr.value) > 0)
        {
            this->interpret_statement(if_stmt_node);
        }
    }
    break;
    case ASTNodeType::OUTPUT:
    {
        // Get Expression
        ExprVal output_expr = this->interpret_expression(statementNode.get_child_nodes()[0]);

        // Display
        std::string output_val = this->runtime_casting(output_expr.type, output_expr.value);
        output_val = this->remove_trailing_zeroes(output_expr);

        std::cout << output_val << std::endl;
    }
    break;
    case ASTNodeType::DECLARATION:
    {
        // Get Identifer
        std::string identifier = statementNode.get_child_nodes()[0].get_value();

        // Get Declared Type
        std::string datatype = statementNode.get_child_nodes()[1].get_value();

        // Create variable
        if (datatype.compare("integer") == 0)
        {
            this->var_map[identifier] = VariableInfo::initialize(ExprValType::INTEGER);
        }
        else if (datatype.compare("double") == 0)
        {
            this->var_map[identifier] = VariableInfo::initialize(ExprValType::DOUBLE);
        }
    }
    break;
    case ASTNodeType::ASSIGNMENT:
    {
        // Get Identifer
        std::string identifier = statementNode.get_child_nodes()[0].get_value();

        // Get value from expression
        ExprVal value_node = this->interpret_expression(statementNode.get_child_nodes()[1]);

        // Check for undeclared variable
        if (this->var_map.find(identifier) == this->var_map.end())
        {
            this->runtime_error("Undeclared variable \"" + identifier + "\"");
        }

        // Assign variable
        this->var_map[identifier] = VariableInfo::assignment(
            this->var_map[identifier],
            this->runtime_casting(
                this->var_map[identifier].data_type, value_node.value));
    }
    break;
    }
}

ExprVal Interpreter::interpret_expression(ASTNode expressionNode)
{
    // Handle nodes with ASTNodeType::NONE
    while (expressionNode.get_node_type() == ASTNodeType::NONE && !expressionNode.get_child_nodes().empty())
    {
        expressionNode = expressionNode.get_child_nodes()[0];
    }

    // Check if atom
    if (expressionNode.get_node_type() == ASTNodeType::ATOM)
    {
        std::string val = this->get_value_if_variable(expressionNode.get_value());
        ExprValType asserted_type = this->assert_type_from_value(val);

        ExprVal expr_val;
        expr_val.value = val;
        expr_val.type = asserted_type;
        return expr_val;
    }

    // Check if binary operation
    if (expressionNode.get_node_type() == ASTNodeType::BINARY_OP)
    {
        // evaluate left and right expression
        ExprVal left_val = this->interpret_expression(expressionNode.get_child_nodes()[0]);
        ExprVal right_val = this->interpret_expression(expressionNode.get_child_nodes()[1]);

        // calculate new value
        std::string calc_val_str;
        ExprValType val_type;
        if (left_val.type == ExprValType::DOUBLE || right_val.type == ExprValType::DOUBLE)
        {
            double calc_val = this->perform_calculation<double>(left_val.value, right_val.value, expressionNode.get_value());
            calc_val_str = std::to_string(calc_val);
            val_type = ExprValType::DOUBLE;
        }
        else
        {
            int calc_val = this->perform_calculation<int>(left_val.value, right_val.value, expressionNode.get_value());
            calc_val_str = std::to_string(calc_val);
            val_type = ExprValType::INTEGER;
        }

        ExprVal expr_val_bin;
        expr_val_bin.type = val_type;
        expr_val_bin.value = calc_val_str;
        return expr_val_bin;
    }

    // Runtime Error
    this->runtime_error("cannot evaluate expression");
    return ExprVal();
}

void Interpreter::runtime_error(std::string content)
{
    std::cout << "Runtime Error: " << content << std::endl;
    std::cout << "Exiting Immediately..." << std::endl;
    exit(1);
}
