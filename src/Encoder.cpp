#include <queue>
#include "Encoder.h"
#include "Node.h"

std::vector<char> Encoder::Encode(std::string input)
{
    auto root = CreateTree(input);
    
    std::vector<CodeEntry> entries = {};
    CalculateCodes(root, 0, 0, entries);

    return std::vector<char>();
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
    { return a->frequency > b->frequency; };
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
        auto left = pq.top();
        pq.pop();
        auto right = pq.top();
        pq.pop();

        auto frequency = left->frequency + right->frequency;
        auto parent = new Node(frequency, left, right);
        pq.push(parent);
    }
    auto root = pq.top();
    pq.pop();

    return root;
}

void Encoder::CalculateCodes(Node* node, int depth, int code, std::vector<CodeEntry>& entries)
{
    if (node->IsLeaf()) 
    {
        CodeEntry entry = {node->value, code, depth};
        entries.push_back(entry);
        return;
    }

    CalculateCodes(node->left, depth + 1, code << 1, entries);
    CalculateCodes(node->right, depth + 1, (code << 1) + 1, entries);
}
