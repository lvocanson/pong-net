#pragma once

#pragma comment(lib, "Ws2_32.lib")
#include <WinSock2.h>
#include <Ws2tcpip.h>

#include <string_view>

namespace NetHelper
{

struct WsaData
{
	WsaData();
	~WsaData();

	operator WSADATA& () { return data; }
	WSADATA* operator&() { return &data; }

	WSADATA data;
	int error;
};

struct UdpAddress;
struct UdpSocket
{
	UdpSocket();
	~UdpSocket();

	bool IsValid() const { return socket != INVALID_SOCKET; }

	int BindTo(UdpAddress&);

	operator SOCKET() { return socket; }
	SOCKET* operator&() { return &socket; }

	SOCKET socket;
};

struct UdpAddress
{
	enum AddrSpecialType
	{
		None = 0,
		Any,
		Broadcast,
		Loopback
	};

	UdpAddress(const char* address);
	UdpAddress(AddrSpecialType address);

	SOCKADDR* operator&() { return (SOCKADDR*)&addr; }
	static consteval int size() { return sizeof(addr); }

	sockaddr_in addr;
};

std::string_view GetWsaErrorExplanation(int error = 0);

} // namespace NetworkHelper
