#include <algorithm>
#include "BitStream.h"
#include "asserts.h"

BitStream::BitStream(std::vector<char>& buffer) : buffer(buffer)
{
    currentBitCount = 0;
}

void BitStream::WriteBits(int value, int numBits)
{
    if (buffer.size() == 0)
    {
        buffer.push_back(0);
    }

    while (numBits > 0)
    {
        auto bitsRemainingInByte = BITS_PER_BYTE - currentBitCount;
        auto numToWrite = std::min(bitsRemainingInByte, numBits);

        auto mask = (1 << numToWrite) - 1;
        auto toWrite = value & mask;

        auto bitsRemainingAfterWrite = bitsRemainingInByte - numToWrite;
        auto toWriteShifted = toWrite << bitsRemainingAfterWrite;

        buffer[buffer.size() - 1] |= toWriteShifted;

        currentBitCount += numToWrite;
        numBits -= numToWrite;
        value >>= numToWrite;

        if (currentBitCount == BITS_PER_BYTE)
        {
            buffer.push_back(0);
            currentBitCount = 0;
        }
    }
}

int BitStream::BitCount()
{
    return currentBitCount;
}