#include "IpAddress.h"
#include <Ws2tcpip.h>
#include <cstring>

inline constexpr u_short DefaultPort = 9551;

IpAddress::IpAddress(const char* address)
	: addr
	{
		.sin_family = AF_INET,
		.sin_port = htons(DefaultPort),
	}
{
	inet_pton(addr.sin_family, address, &addr.sin_addr.s_addr);
}

IpAddress::IpAddress(AddrSpecialType address)
	: addr
	{
		.sin_family = AF_INET,
		.sin_port = htons(DefaultPort),
	}
{
	switch (address)
	{
	case None:
		addr.sin_addr.s_addr = INADDR_NONE;
		break;
	case Any:
		addr.sin_addr.s_addr = INADDR_ANY;
		break;
	case Broadcast:
		addr.sin_addr.s_addr = INADDR_BROADCAST;
		break;
	case Loopback:
		addr.sin_addr.s_addr = INADDR_LOOPBACK;
		break;
	}
}

void IpAddress::ToString(String buffer) const
{
	static_assert(INET_ADDRSTRLEN == sizeof(String));
	inet_ntop(AF_INET, &(addr.sin_addr), buffer, INET_ADDRSTRLEN);
}

bool IpAddress::operator==(const IpAddress& lhs) const
{
	return std::memcmp(&addr, &lhs.addr, size()) == 0;
}
