#include "call_stack.hpp"

CallStack::CallStack(ASTNode node, int depth) : Interpreter(node)
{
    this->function_node = node;
    this->depth = depth;
}

CallStack::~CallStack()
{
}

ExprVal CallStack::evaluate_function()
{
    ExprVal function_eval;


    // Setup Type
    ASTNode func_type_node = this->function_node.get_child_nodes()[0];
    if (func_type_node.get_value().compare("integer") == 0)
    {
        function_eval.type = ExprValType::INTEGER;
    }
    else if (func_type_node.get_value().compare("double") == 0)
    {
        function_eval.type = ExprValType::DOUBLE;
    }


    // Setup Value
    

    return function_eval;
}
