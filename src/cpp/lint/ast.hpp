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

    ATOM
};

class ASTNode {
public:
    ASTNode();
    ~ASTNode();

    void set_node_type(ASTNodeType _node_type);
    void set_value(std::string _value);
    void add_child_node(ASTNode *_child_node);
private:
    ASTNodeType node_type;
    std::string value;
    std::vector<ASTNode*> child_nodes;
};