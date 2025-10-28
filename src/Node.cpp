#include "Node.h"

#define DUMMY_CHAR -1

Node::Node(char value, int frequency) : Node(value, frequency, nullptr, nullptr)
{
}

Node::Node(int frequency, Node *left, Node *right) : Node(DUMMY_CHAR, frequency, left, right)
{
}

bool Node::IsLeaf()
{
    return left == nullptr && right == nullptr;
}

Node::Node(char value, int frequency, Node *left, Node *right)
{
    this->value = value;
    this->frequency = frequency;
    this->left = left;
    this->right = right;
}
