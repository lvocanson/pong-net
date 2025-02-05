#pragma once

#include "Packet.h"
#include <string_view>

namespace NetHelper
{

std::string_view GetWsaErrorExplanation(int error = 0);

} // namespace NetworkHelper
