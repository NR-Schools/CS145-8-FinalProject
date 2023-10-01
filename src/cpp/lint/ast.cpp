#include "ast.hpp"

ASTNode::ASTNode()
{
}

ASTNode::~ASTNode()
{
}

void ASTNode::set_node_type(ASTNodeType _node_type)
{
    this->node_type = _node_type;
}

void ASTNode::set_value(std::string _value)
{
    this->value = _value;
}

void ASTNode::add_child_node(ASTNode *_child_node)
{
    this->child_nodes.push_back(_child_node);
}
