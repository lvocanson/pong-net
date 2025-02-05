#include "Misc.h"
#include <random>
#include <sstream>

namespace Misc
{

Generator<unsigned> FibonacciGen()
{
    unsigned a = 0, b = 1;
    while (true)
    {
        co_yield a;
        unsigned next = a + b;
        a = b;
        b = next;
    }
}

uint16_t GenerateUUID()
{
	std::random_device rd;
	std::mt19937 gen(rd());
	std::uniform_int_distribution<uint16_t> dist;
	return dist(gen);
}

} // namespace Misc
