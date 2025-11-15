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
    std::vector<CodeEntry> entries = {};
    auto index = DeserializeEntries(input, entries);
    auto root = CreateTreeFromEntries(entries);
    auto decoded = DecodeInput(input, index, root);

    return decoded;
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

        CodeEntry entry = {node->value, code, (char)codeLength};
        entries.push_back(entry);
        return;
    }

    CalculateCodes(node->left, depth + 1, code << 1, entries);
    CalculateCodes(node->right, depth + 1, (code << 1) + 1, entries);
}

std::map<char, CodeEntry> Encoder::CreateEntryMap(std::vector<CodeEntry> &entries)
{
    std::map<char, CodeEntry> codeEntryByValue = {};
    for (int i = 0; i < entries.size(); i++)
    {
        auto entry = entries[i];
        codeEntryByValue[entry.value] = entry;
    }

    return codeEntryByValue;
}

void Encoder::WriteBytes(int value, std::vector<char> &buffer)
{
    // Little endian
    buffer.push_back(value & 0xFF);
    buffer.push_back((value >> 8) & 0xFF);
    buffer.push_back((value >> 16) & 0xFF);
    buffer.push_back((value >> 24) & 0xFF);
}

void Encoder::SerializeEntries(std::vector<CodeEntry> &entries, std::vector<char> &buffer)
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

std::vector<char> Encoder::EncodeInput(std::string input, std::vector<CodeEntry> &entries, std::map<char, CodeEntry> &codeEntryByValue)
{
    std::vector<char> buffer = {};

    SerializeEntries(entries, buffer);
    
    std::vector<char> bitStreamBuffer = {};
    auto bitStream = new BitStream(bitStreamBuffer);
    for (int i = 0; i < input.size(); i++)
    {
        auto c = input[i];
        auto entry = codeEntryByValue[c];
        bitStream->WriteBits(entry.code, entry.codeLength);
    }

    // Keep track of the number of bits written to the final byte by including it in the header.
    buffer.push_back(bitStream->BitCount());

    // Append the data.
    buffer.insert(buffer.end(), bitStreamBuffer.begin(), bitStreamBuffer.end());

    return buffer;
}

int Encoder::ReadInt(std::vector<char> &input, int index)
{
    int result = input[index] | (input[index + 1] << 8) | (input[index + 2] << 16) | (input[index + 3] << 24);

    return result;
}

CodeEntry Encoder::ReadEntry(std::vector<char> &input, int index)
{
    char value = input[index];
    int code = ReadInt(input, index + 1);
    char codeLength = input[index + 5];

    return {value, code, codeLength};
}

int Encoder::DeserializeEntries(std::vector<char> &input, std::vector<CodeEntry> &entries)
{
    int count = ReadInt(input, 0);
    int inputIndex = sizeof(int);

    for (int i = 0; i < count; i++)
    {
        auto entry = ReadEntry(input, inputIndex);
        entries.push_back(entry);
        inputIndex += sizeof(CodeEntry);
    }

    return inputIndex;
}

Node *Encoder::CreateTreeFromEntries(std::vector<CodeEntry> &entries)
{
    auto root = new Node();

    for (int i = 0; i < entries.size(); i++)
    {
        auto entry = entries[i];
        CreatePathFromEntry(root, entry);
    }

    return root;
}

void Encoder::CreatePathFromEntry(Node *root, CodeEntry entry)
{
    auto node = root;

    int code = entry.code;
    char numBits = entry.codeLength;
    while (numBits > 0)
    {
        char bit = code >> (numBits - 1);
        if (bit)
        {
            if (node->right == nullptr)
            {
                node->right = new Node();
            }
            node = node->right;
        }
        else 
        {
            if (node->left == nullptr)
            {
                node->left = new Node();
            }
            node = node->left;
        }

        if (numBits == 1)
        {
            node->value = entry.value;
        }

        numBits--;
        code >>= 1;
    }
}

std::string Encoder::DecodeInput(std::vector<char> &input, int index, Node *root)
{
    std::vector<char> result = {};
    int numBitsInFinalByte = input[index];

    auto node = root;
    for (int i = index + 1; i < input.size(); i++)
    {
        char numBits = 8; 
        if (index == input.size() - 1) 
        {
            numBits = numBitsInFinalByte;
        }

        // Not at all optimized approach: consume one bit at a time and walk the tree.
        char value = input[i];
        while (numBits > 0)
        {
            char bit = value >> (numBits - 1);
            if (bit)
            {
                node = node->right;
            }
            else 
            {
                node = node->left;
            }

            if (node->IsLeaf())
            {
                result.push_back(node->value);
                node = root;
            }

            numBits--;
            value >>= 1;
        }
    }

    auto decoded = std::string(result.begin(), result.end());
    return decoded;
}