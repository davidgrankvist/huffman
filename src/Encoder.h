#ifndef Encoder_h
#define Encoder_h

#include <vector>
#include <string>

class Node;

class Encoder
{
public:
    std::vector<char> Encode(std::string input);
    std::string Decode(std::vector<char> input);
private:
    Node* CreateTree(std::string input);
};

#endif