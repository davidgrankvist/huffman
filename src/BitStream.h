#ifndef BitStream_h
#define BitStream_h

#include <vector>

class BitStream
{
    public:
        BitStream(std::vector<char>& buffer);
        // Treat input as an n-bit value and write those bits to the buffer.
        void WriteBits(int value, int numBits);
    private:
        // Bit count within the byte that's currently being written to.
        int currentBitCount;
        std::vector<char> &buffer;
};

#endif