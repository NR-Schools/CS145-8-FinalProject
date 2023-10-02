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

template <typename T, typename>
T Interpreter::perform_calculation(std::string left, std::string right, std::string op)
{
    T left_val = (T)std::stod(left);
    T right_val = (T)std::stod(right);

    if (op.compare("+") == 0) return left_val + right_val;
    else if (op.compare("-") == 0) return left_val - right_val;

    else if (op.compare("==") == 0) return left_val == right_val;
    else if (op.compare("!=") == 0) return left_val != right_val;
    else if (op.compare(">") == 0) return left_val > right_val;
    else if (op.compare(">=") == 0) return left_val >= right_val;
    else if (op.compare("<") == 0) return left_val < right_val;
    else if (op.compare("<=") == 0) return left_val <= right_val;

    return 0;
}


std::string Interpreter::get_value_if_variable(std::string unknown_atom)
{
    // If variable -> return var value, else, just return value
    if (this->var_map.find(unknown_atom) != this->var_map.end())
    {
        return this->var_map[unknown_atom].value;
    }
    return unknown_atom;
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
        std::cout << this->runtime_casting(output_expr.type, output_expr.value) << std::endl;
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

        // Assign variable
        this->var_map[identifier] = VariableInfo::assignment(
            this->var_map[identifier],
            this->runtime_casting(
                this->var_map[identifier].data_type, value_node.value
            )
        );
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
}

void Interpreter::runtime_error(std::string content)
{
}
