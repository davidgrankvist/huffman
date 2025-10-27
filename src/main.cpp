#include <iostream>
#include <string>
#include "Encoder.h"

int main()
{
    std::string input = "hello";

    auto encoder = new Encoder();
    auto encoded = encoder->Encode(input);
    auto decoded = encoder->Decode(encoded);

    std::cout << decoded << std::endl;
    return 0;
}