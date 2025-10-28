#ifndef Encoder_h
#define Encoder_h

#include <vector>
#include <string>

class Node;

struct CodeEntry
{
    char value;
    int code;
    int codeLength;
};

class Encoder
{
public:
    std::vector<char> Encode(std::string input);
    std::string Decode(std::vector<char> input);
private:
    Node* CreateTree(std::string input);
    void CalculateCodes(Node* node, int depth, int code, std::vector<CodeEntry>& entries);
};

#endif