#pragma once

#include "interpreter.hpp"
#include "../lint/ast.hpp"

class CallStack : InterpreterTools
{
public:
    CallStack(std::unordered_map<std::string, FunctionInfo> func_map, FunctionInfo func_info, std::vector<ExprVal> arg_list, int depth);
    ~CallStack();

    ExprVal evaluate_function();

    void declare_function(ASTNode node);
    ExprVal interpret_function(ASTNode node);
    void return_function(ASTNode node);
public:
    ExprVal return_value;
    FunctionInfo func_info;
    int curr_depth;
    bool is_value_received;
};