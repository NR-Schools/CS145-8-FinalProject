#pragma once

#include <iostream>
#include <vector>
#include <stdexcept>
#include <unordered_map>
#include <memory>

enum ASTNodeType {
    PROGRAM,

    DECLARATION,
    ASSIGNMENT,
    IF,
    OUTPUT,

    BINARY_OP,
    UNARY_OP,

    ATOM,

    NONE
};

class ASTNode {
public:
    ASTNode();
    ~ASTNode();

    void set_node_type(ASTNodeType _node_type);
    void set_value(std::string _value);
    void add_child_node(ASTNode _child_node);

    ASTNodeType get_node_type();
    std::string get_value();
    std::vector<ASTNode> get_child_nodes();
private:
    ASTNodeType node_type;
    std::string value;
    std::vector<ASTNode> child_nodes;
};