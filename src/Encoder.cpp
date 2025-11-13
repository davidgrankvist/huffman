#include <queue>
#include <map>
#include "asserts.h"
#include "Encoder.h"
#include "Node.h"
#include "BitStream.h"

std::vector<char> Encoder::Encode(std::string input)
{
    auto root = CreateTree(input);

    std::vector<CodeEntry> entries = {};
    CalculateCodes(root, 0, 0, entries);

    auto codeEntryByValue = CreateEntryMap(entries);
    auto encoded = EncodeInput(input, entries, codeEntryByValue);

    return encoded;
}

std::string Encoder::Decode(std::vector<char> input)
{
    return std::string();
}

Node *Encoder::CreateTree(std::string input)
{
    const int numSymbols = 256;
    int frequencies[numSymbols] = {};

    // Calculate frequencies
    for (int i = 0; i < input.length(); i++)
    {
        char c = input.at(i);
        frequencies[c]++;
    }

    // PQ with low frequencies at the top
    auto cmp = [](Node *a, Node *b) -> bool
    { return a->frequency == b->frequency ? a->maxLeafFrequency > b->maxLeafFrequency
                                          : a->frequency > b->frequency; };
    std::priority_queue<Node *, std::vector<Node *>, decltype(cmp)> pq(cmp);

    // Create leaf nodes
    for (int i = 0; i < numSymbols; i++)
    {
        char value = (char)i;
        int frequency = frequencies[i];

        if (frequency == 0)
        {
            continue;
        }

        auto node = new Node(value, frequency);
        pq.push(node);
    }

    // Construct tree by connecting nodes
    while (pq.size() > 1)
    {
        auto right = pq.top();
        pq.pop();
        auto left = pq.top();
        pq.pop();

        auto parent = new Node(left, right);
        pq.push(parent);
    }
    auto root = pq.top();
    pq.pop();

    return root;
}

void Encoder::CalculateCodes(Node *node, int depth, int code, std::vector<CodeEntry> &entries)
{
    if (node->IsLeaf())
    {
        auto codeLength = depth;
        Assert(codeLength <= 32, "Codes are assumed to be at most 32 bits.");

        CodeEntry entry = {node->value, code, codeLength};
        entries.push_back(entry);
        return;
    }

    CalculateCodes(node->left, depth + 1, code << 1, entries);
    CalculateCodes(node->right, depth + 1, (code << 1) + 1, entries);
}

std::map<char, CodeEntry> Encoder::CreateEntryMap(std::vector<CodeEntry> entries)
{
    std::map<char, CodeEntry> codeEntryByValue = {};
    for (int i = 0; i < entries.size(); i++) 
    {
        auto entry = entries[i];
        codeEntryByValue[entry.value] = entry;
    }

    return codeEntryByValue;
}

void Encoder::WriteBytes(int value, std::vector<char> buffer)
{
    buffer.push_back(value & 0xFF);
    buffer.push_back((value >> 8) & 0xFF);
    buffer.push_back((value >> 16) & 0xFF);
    buffer.push_back((value >> 24) & 0xFF);
}

void Encoder::SerializeEntries(std::vector<CodeEntry> entries, std::vector<char> buffer)
{
    int count = entries.size();
    WriteBytes(count, buffer);

    for (int i = 0; i < count; i++)
    {
        auto entry = entries[i];
        buffer.push_back(entry.code);
        WriteBytes(entry.code, buffer);
        buffer.push_back(entry.codeLength);
    }
}

std::vector<char> Encoder::EncodeInput(std::string input, std::vector<CodeEntry> entries, std::map<char, CodeEntry> codeEntryByValue)
{
    std::vector<char> buffer = {};

    SerializeEntries(entries, buffer);

    auto bitStream = new BitStream(buffer);
    for (int i = 0; i < input.size(); i++) 
    {
        auto c = input[i];
        auto entry = codeEntryByValue[c];
        bitStream->WriteBits(entry.code, entry.codeLength);
    }

    return buffer;
}