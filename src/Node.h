#ifndef Node_h
#define Node_h

class Node
{
public:
    char value;
    int frequency;
    Node *left;
    Node *right;
    Node(char value, int frequency);
    Node(int frequency, Node *left, Node *right);
private:
    Node(char value, int frequency, Node *left, Node *right);
};

#endif