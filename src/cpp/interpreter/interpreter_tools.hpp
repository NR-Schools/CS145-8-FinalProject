#pragma once

#include <iostream>
#include <sstream>
#include <string>
#include <type_traits>
#include <unordered_map>

#include "../lint/ast.hpp"

enum ExprValType
{
    INTEGER,
    DOUBLE
};

struct ExprVal
{
    std::string value;
    ExprValType type;
};

struct VariableInfo
{
    ExprValType data_type;
    std::string value;

    static VariableInfo initialize(ExprValType type)
    {
        VariableInfo v_info;
        v_info.data_type = type;
        v_info.value = "0";
        return v_info;
    }

    static VariableInfo assignment(VariableInfo v_info, std::string new_value)
    {
        v_info.value = new_value;
        return v_info;
    }
};

struct FunctionParameters
{
    std::unordered_map<std::string, VariableInfo> local_var_map;
    std::vector<std::string> insertion_order;
};

struct FunctionInfo
{
    FunctionParameters func_params;
    ExprValType func_return_type;
    ASTNode statements_holder;
};

class InterpreterTools
{
public:
    std::unordered_map<std::string, VariableInfo> var_map;
    std::unordered_map<std::string, FunctionInfo> func_map;

public:
    std::string runtime_casting(ExprValType type, std::string value);
    bool is_valid_value(std::string value_str);

    template <typename T, typename = typename std::enable_if<std::is_same<T, int>::value ||
                                                             std::is_same<T, double>::value>::type>
    T perform_calculation(std::string left, std::string right, std::string op);

    template <typename T, typename = typename std::enable_if<std::is_same<T, int>::value ||
                                                             std::is_same<T, double>::value>::type>
    T calculated_expression_formatter(T val);

    std::string remove_trailing_zeroes(ExprVal value);

    ExprVal get_value_if_variable(std::string unknown_atom);
    ExprValType __assert_type_from_value(std::string unknown_value);

    void interpret_statement(ASTNode statementNode);
    ExprVal interpret_expression(ASTNode expressionNode);

    void runtime_error(std::string content);

    void check_if_declared(std::string symbol_name);

public:
    virtual void declare_function(ASTNode node) = 0;
    virtual ExprVal interpret_function(ASTNode node) = 0;
    virtual void return_function(ASTNode node) = 0;
};