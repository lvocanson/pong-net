#include "Packet.h"

inline constexpr uint16_t HeaderSecret = 0xD892;

void Packet::Sign(uint16_t signature)
{
	header.secret = HeaderSecret;
	header.signature = signature;
}

bool Packet::IsValid() const
{
	return header.secret == HeaderSecret
		&& header.index < header.total;
}
