#ifndef Encoder_h
#define Encoder_h

#include <vector>
#include <string>
#include <map>

class Node;

struct CodeEntry
{
    char value;
    int code;
    char codeLength;
};

class Encoder
{
public:
    std::vector<char> Encode(std::string input);
    std::string Decode(std::vector<char> input);
private:
    Node* CreateTree(std::string input);
    void CalculateCodes(Node* node, int depth, int code, std::vector<CodeEntry>& entries);
    std::map<char, CodeEntry> CreateEntryMap(std::vector<CodeEntry> entries);
    void WriteBytes(int value, std::vector<char> buffer);
    void SerializeEntries(std::vector<CodeEntry> entries, std::vector<char> buffer);
    std::vector<char> EncodeInput(std::string input, std::vector<CodeEntry> entries, std::map<char, CodeEntry> codeEntryByValue);
};

#endif