#include "interpreter.hpp"

Interpreter::Interpreter(ASTNode root, int max_allowed_depth)
{
    this->root = root;
    this->max_allowed_depth = max_allowed_depth;
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

void Interpreter::declare_function(ASTNode node)
{
    // Store info on func_map
    std::string func_name;
    FunctionInfo func_info;

    // Get Function Name
    func_name = node.get_value();

    // Check if already declared
    check_if_declared(func_name);

    // Get Other Function Info
    for (ASTNode subnode : node.get_child_nodes())
    {
        // Get Function Parameters
        if (subnode.get_node_type() == ASTNodeType::FUNC_PARAM_LIST)
        {
            // Loop each declaration
            for (ASTNode decl_node : subnode.get_child_nodes())
            {
                // Process parameter declaration
                VariableInfo var_info;

                std::string identifier = decl_node.get_child_nodes()[0].get_value();
                std::string datatype = decl_node.get_child_nodes()[1].get_value();

                // Push to Map
                if (datatype.compare("integer") == 0)
                    func_info.func_params.local_var_map[identifier] = VariableInfo::initialize(ExprValType::INTEGER);
                else if (datatype.compare("double") == 0)
                    func_info.func_params.local_var_map[identifier] = VariableInfo::initialize(ExprValType::DOUBLE);
                
                // Push to Vector
                func_info.func_params.insertion_order.push_back(identifier);
            }
        }

        // Get Return Type
        if (subnode.get_node_type() == ASTNodeType::ATOM)
        {
            std::string val_type = subnode.get_value();

            if (val_type.compare("integer") == 0)
                func_info.func_return_type = ExprValType::INTEGER;
            else if (val_type.compare("double") == 0)
                func_info.func_return_type = ExprValType::DOUBLE;
        }

        // Get Statement List
        if (subnode.get_node_type() == ASTNodeType::FUNC_IMPL)
        {
            func_info.statements_holder = subnode;
        }
    }

    // Store on func_map
    this->func_map[func_name] = func_info;
}

ExprVal Interpreter::interpret_function(ASTNode node)
{
    // Get Function Name
    std::string func_name = node.get_value();

    // Get Function Parameters
    std::vector<ExprVal> arg_list;
    for (ASTNode subnode : node.get_child_nodes())
    {
        // Check Expression
        ExprVal expr_val = this->interpret_expression(subnode);
        arg_list.push_back(expr_val);
    }

    // Process Function via CallStack
    auto iter = this->func_map.find(func_name);
    if (iter != this->func_map.end())
    {
        FunctionInfo func_info = iter->second;
        CallStack call_stack(func_name, this->func_map, func_info, arg_list, this->max_allowed_depth);
        ExprVal expr_val = call_stack.evaluate_function();
        return expr_val;
    }
    else
        this->runtime_error("Error: function \"" + func_name + "\" not found");
    
    return ExprVal();
}

void Interpreter::return_function(ASTNode node)
{
    this->runtime_error("Invalid use of return statement!");
}
