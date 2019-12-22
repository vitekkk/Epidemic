#ifndef RANDOMTOOLKIT_H
#define RANDOMTOOLKIT_H

#include <random>

class RandomToolkit
{
    std::random_device _rd;
    std::mt19937 _gen;
    RandomToolkit();
    static RandomToolkit& getToolkit();
public:
    static std::mt19937& getMersienneGenerator();
};

#endif // RANDOMTOOLKIT_H
