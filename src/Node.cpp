#include <algorithm>
#include "Node.h"

#define DUMMY_CHAR -1

Node::Node(char value, int frequency) : Node(value, frequency, nullptr, nullptr)
{
    this->maxLeafFrequency = frequency;
}

Node::Node(Node *left, Node *right) : Node(DUMMY_CHAR, left->frequency + right->frequency, left, right)
{
    this->maxLeafFrequency = std::max(left->maxLeafFrequency, right->maxLeafFrequency);
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
