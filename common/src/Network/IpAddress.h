#pragma once
#include <WinSock2.h>

struct IpAddress
{
	enum AddrSpecialType
	{
		None = 0,
		Any,
		Broadcast,
		Loopback
	};

	using String = char[22];

	IpAddress(const char* address);
	IpAddress(AddrSpecialType address = None);

	void ToString(String buffer) const;

	bool operator==(const IpAddress& lhs) const;

	SOCKADDR* operator&() { return (SOCKADDR*)&addr; }
	const SOCKADDR* operator&() const { return (SOCKADDR*)&addr; }
	static consteval int size() { return sizeof(addr); }

	sockaddr_in addr;
};
