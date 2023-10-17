#include "call_stack.hpp"

CallStack::CallStack(std::string func_name, std::unordered_map<std::string, FunctionInfo> func_map, FunctionInfo func_info, std::vector<ExprVal> arg_list, int depth)
{
    this->func_name = func_name;
    this->func_map = func_map;
    this->func_info = func_info;
    this->curr_depth = depth - 1;
    this->is_value_received = false;

    // Check depth
    if (curr_depth < 0)
        this->runtime_error("Allowed Call Stack Exceeded!");

    // Check if arguments matches parameters (size)
    if (this->func_info.func_params.insertion_order.size() != arg_list.size())
        this->runtime_error(
            "Function \"" +
            this->func_name +
            "\" expects [" +
            std::to_string(this->func_info.func_params.insertion_order.size()) +
            "] argument(s) but received [" +
            std::to_string(arg_list.size()) +
            "]"
        );

    this->var_map = func_info.func_params.local_var_map;
    this->func_map = func_map;

    // Assign values to parameter-variables
    for (int i = 0; i < func_info.func_params.insertion_order.size(); i++)
    {
        std::string param = func_info.func_params.insertion_order[i];
        std::string value = arg_list[i].value;

        // Assign variable
        this->var_map[param] = VariableInfo::assignment(
            this->var_map[param],
            this->runtime_casting(
                this->var_map[param].data_type,
                value));
    }
}

CallStack::~CallStack()
{
}

ExprVal CallStack::evaluate_function()
{
    // Execute Statements line by line
    for (ASTNode statement_node : this->func_info.statements_holder.get_child_nodes())
    {
        this->interpret_statement(statement_node);

        if (this->is_value_received)
            break;
    }

    // Check for value
    if (!this->is_value_received)
        this->runtime_error("Function exited without returning a value");

    std::string adjusted_value = this->runtime_casting(
        this->func_info.func_return_type,
        this->return_value.value
    );

    ExprVal ret_expr_val;
    ret_expr_val.type = this->func_info.func_return_type;
    ret_expr_val.value = adjusted_value;

    return ret_expr_val;
}

void CallStack::declare_function(ASTNode node)
{
    this->runtime_error("Function Declaration inside a function is not allowed!");
}

ExprVal CallStack::interpret_function(ASTNode node)
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
        CallStack call_stack(func_name, this->func_map, func_info, arg_list, this->curr_depth);
        return call_stack.evaluate_function();
    }
    else
        this->runtime_error("Error: function \"" + func_name + "\" not found");
    
    return ExprVal();
}

void CallStack::return_function(ASTNode node)
{
    this->is_value_received = true;
    this->return_value = this->interpret_expression(node.get_child_nodes()[0]);
}
