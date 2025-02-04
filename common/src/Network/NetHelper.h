#pragma once

#include "NetMessage.h"
#include <string_view>

#pragma comment(lib, "Ws2_32.lib")
#include <WinSock2.h>
#include <Ws2tcpip.h>

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

	int BindTo(const UdpAddress&) const;

	template <NetMessageType T>
	bool Send(const NetMessage<T>& message, const UdpAddress& recipient) const
	{
		auto header = NetHeader::For<T>();
		if (!Send(reinterpret_cast<char*>(&header), sizeof(NetHeader), recipient))
			return false;

		return Send(reinterpret_cast<const char*>(&message), sizeof(NetMessage<T>), recipient);
	};

	bool Send(const char* data, int size, const UdpAddress& recipient) const;

	bool CheckPendingMessage(long delay = 500) const;
	bool ReceiveMessage(NetHeader& outHeader, NetMessage<NetMessageType::Unknown>& outMessage, UdpAddress& outSender) const;

	operator SOCKET() const { return socket; }
	SOCKET* operator&() { return &socket; }
	const SOCKET* operator&() const { return &socket; }

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
	const SOCKADDR* operator&() const { return (SOCKADDR*)&addr; }
	static consteval int size() { return sizeof(addr); }

	sockaddr_in addr;
};

std::string_view GetWsaErrorExplanation(int error = 0);

} // namespace NetworkHelper
