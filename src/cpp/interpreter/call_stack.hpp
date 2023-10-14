#pragma once

#include "interpreter.hpp"
#include "../lint/ast.hpp"

class CallStack : public Interpreter
{
public:
    CallStack(ASTNode node, int depth);
    ~CallStack();

private:
    ExprVal evaluate_function();
    bool has_error;
    int depth;

private:
    ASTNode function_node;
};