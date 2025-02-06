#pragma once
#define NOMINMAX
#include <WinSock2.h>
#include <string_view>

// More user friendly address (base 64)
struct IpPhrase
{
	IpPhrase();
	IpPhrase(std::string_view src);
	std::string_view View() const;
	
	char buffer[6];
};

struct IpAddress
{
	enum AddrSpecialType
	{
		None = 0,
		Any,
		Broadcast,
		Loopback
	};

	IpAddress(AddrSpecialType address = None);
	IpAddress(IpPhrase phrase);
	static IpAddress LocalAddress();
	
	IpPhrase ToPhrase() const;

	bool operator==(const IpAddress& lhs) const;

	SOCKADDR* operator&() { return (SOCKADDR*)&addr; }
	const SOCKADDR* operator&() const { return (SOCKADDR*)&addr; }
	static consteval int size() { return sizeof(addr); }

	sockaddr_in addr;
};
