#include <iostream>
#include <sstream>
#include <string>
#include <type_traits>
#include <unordered_map>

#include "../lint/ast.hpp"


enum ExprValType {
    INTEGER,
    DOUBLE
};

struct ExprVal {
    std::string value;
    ExprValType type;
};

struct VariableInfo {
    ExprValType data_type;
    std::string value;

    static VariableInfo initialize(ExprValType type) {
        VariableInfo v_info;
        v_info.data_type = type;
        v_info.value = "0";
        return v_info;
    }

    static VariableInfo assignment(VariableInfo v_info, std::string new_value) {
        v_info.value = new_value;
        return v_info;
    }
};

class Interpreter
{
public:
    Interpreter(ASTNode root);
    ~Interpreter();
    
    void interpret();

private:
    ASTNode root;
    std::unordered_map<std::string, VariableInfo> var_map;

    std::string runtime_casting(ExprValType type, std::string value);
    bool is_valid_value(std::string value_str);
    
    template <typename T, typename = typename std::enable_if<std::is_same<T, int>::value ||
                                              std::is_same<T, double>::value>::type>
    T perform_calculation(std::string left, std::string right, std::string op);

    template <typename T, typename = typename std::enable_if<std::is_same<T, int>::value ||
                                              std::is_same<T, double>::value>::type>
    T calculated_expression_formatter(T val);
    
    std::string remove_trailing_zeroes(ExprVal value);

    std::string get_value_if_variable(std::string unknown_atom);
    ExprValType assert_type_from_value(std::string unknown_value);

    void interpret_statement(ASTNode statementNode);
    ExprVal interpret_expression(ASTNode expressionNode);

    void runtime_error(std::string content);
};