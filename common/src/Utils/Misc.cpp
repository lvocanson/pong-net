#include "Misc.h"
#include <random>
#include <sstream>

namespace Misc
{

uint16_t GenerateUUID()
{
	std::random_device rd;
	std::mt19937 gen(rd());
	std::uniform_int_distribution<uint16_t> dist;
	return dist(gen);
}

} // namespace Misc
