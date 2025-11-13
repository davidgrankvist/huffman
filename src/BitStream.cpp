#include "BitStream.h"
#include "asserts.h"

BitStream::BitStream(std::vector<char> buffer)
{
    this->buffer = buffer;
}

void BitStream::WriteBits(int value, int numBits)
{
    AssertTodo();
}