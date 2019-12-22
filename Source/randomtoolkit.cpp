#include "randomtoolkit.h"
#include <chrono>

RandomToolkit::RandomToolkit():
    _gen(_rd())
{
    _gen.seed(std::chrono::high_resolution_clock::now().time_since_epoch().count());
}

RandomToolkit& RandomToolkit::getToolkit()
{
    static RandomToolkit rt;
    return rt;
}

std::mt19937& RandomToolkit::getMersienneGenerator()
{
    return getToolkit()._gen;
}
