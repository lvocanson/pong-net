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
	std::ptrdiff_t decoded = 0;
	for (int i = 0; i < 6; ++i)
	{
		const char* pos = strchr(Base64Alphabet, phrase.buffer[i]);
		if (pos)
		{
			decoded = (decoded << 6) | pos - Base64Alphabet;
		}
	}

	addr.sin_addr.s_addr = htonl(static_cast<u_long>(decoded));
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

IpAddress IpAddress::PublicAddress()
{
	addrinfo hints = {};
	hints.ai_family = AF_INET;
	hints.ai_socktype = SOCK_STREAM;

	addrinfo* result;
	if (getaddrinfo("api.ipify.org", "80", &hints, &result) != 0)
		return {};

	SOCKET sock = socket(result->ai_family, result->ai_socktype, result->ai_protocol);
	if (sock == INVALID_SOCKET)
		return {};

	if (connect(sock, result->ai_addr, static_cast<int>(result->ai_addrlen)) == SOCKET_ERROR)
		return {};

	freeaddrinfo(result);

	const char* request = "GET / HTTP/1.1\r\nHost: api.ipify.org\r\nConnection: close\r\n\r\n";
	send(sock, request, (int)strlen(request), 0);

	char buffer[1024] = {};
	recv(sock, buffer, sizeof(buffer) - 1, 0);
	closesocket(sock);

	char* ipStart = strstr(buffer, "\r\n\r\n");
	if (!ipStart)
		return {};

	char* ip = ipStart + 4;
	addrinfo hints2 = {}, * ipRes;
	hints2.ai_family = AF_INET;
	hints2.ai_socktype = SOCK_STREAM;

	if (getaddrinfo(ip, nullptr, &hints2, &ipRes) != 0)
		return {};

	IpAddress wideAddress;
	wideAddress.addr = *reinterpret_cast<sockaddr_in*>(ipRes->ai_addr);

	freeaddrinfo(ipRes);
	return wideAddress;
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
