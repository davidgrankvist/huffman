#ifndef BitStream_h
#define BitStream_h

#include <vector>

class BitStream
{
    public:
        BitStream(std::vector<char> buffer);
        void WriteBits(int value, int numBits);
    private:
        int currentBit;
        std::vector<char> buffer;
};

#endif