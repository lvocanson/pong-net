#include "IpAddress.h"
#include <Ws2tcpip.h>
#include <cstring>
#include <cstdint>

IpPhrase::IpPhrase()
	: buffer{}
{
}

IpPhrase::IpPhrase(std::string_view src)
{
	std::memcpy(buffer, src.data(), std::min(src.size(), sizeof(buffer)));
}

std::string_view IpPhrase::View() const
{
	return {buffer, sizeof(buffer)};
}

inline constexpr u_short DefaultPort = 9551;

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

inline constexpr char Base64Alphabet[65] = "ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789+-";

IpAddress::IpAddress(IpPhrase phrase)
	: addr
	{
		.sin_family = AF_INET,
		.sin_port = htons(DefaultPort),
	}
{
	uint32_t decoded = 0;
	for (int i = 0; i < 6; ++i)
	{
		const char* pos = strchr(Base64Alphabet, phrase.buffer[i]);
		if (pos)
		{
			decoded = (decoded << 6) | (pos - Base64Alphabet);
		}
	}

	addr.sin_addr.s_addr = htonl(decoded);
}

IpAddress IpAddress::LocalAddress()
{
	char hostName[256];
	if (gethostname(hostName, 256) != 0)
		return {};

	addrinfo hints{};
	hints.ai_family = AF_INET;
	hints.ai_socktype = SOCK_STREAM;

	addrinfo* result;
	int status = getaddrinfo(hostName, nullptr, &hints, &result);
	if (status != 0)
	{
		freeaddrinfo(result);
		return {};
	}

	IpAddress localAddress;
	localAddress.addr = *reinterpret_cast<sockaddr_in*>(result->ai_addr);

	freeaddrinfo(result);
	return localAddress;
}

IpPhrase IpAddress::ToPhrase() const
{
	IpPhrase phrase{};
	uint32_t ip = ntohl(addr.sin_addr.s_addr);

	for (int i = 5; i >= 0; --i)
	{
		phrase.buffer[i] = Base64Alphabet[ip & 0x3F];
		ip >>= 6;
	}

	return phrase;
}

bool IpAddress::operator==(const IpAddress& lhs) const
{
	return std::memcmp(&addr, &lhs.addr, size()) == 0;
}
