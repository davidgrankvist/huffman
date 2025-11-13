#ifndef Node_h
#define Node_h

class Node
{
public:
    char value;
    int frequency;
    int maxLeafFrequency;
    Node *left;
    Node *right;
    // Create a leaf node
    Node(char value, int frequency);
    // Create a parent node, combining the children frequencies
    Node(Node *left, Node *right);
    bool IsLeaf();
private:
    // Internal constructor to create either a leaf or a parent
    Node(char value, int frequency, Node *left, Node *right);
};

#endif