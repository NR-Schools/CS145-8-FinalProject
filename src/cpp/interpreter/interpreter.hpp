#pragma once

#include <iostream>
#include <sstream>
#include <string>
#include <type_traits>
#include <unordered_map>

#include "interpreter_tools.hpp"
#include "call_stack.hpp"
#include "../lint/ast.hpp"


class Interpreter : InterpreterTools
{
public:
    Interpreter(ASTNode root);
    ~Interpreter();
    
    void interpret();

    void declare_function(ASTNode node);
    ExprVal interpret_function(ASTNode node);
    void return_function(ASTNode node);

private:
    ASTNode root;
};