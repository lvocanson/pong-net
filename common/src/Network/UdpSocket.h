#pragma once
#include <WinSock2.h>
#include <Game/Message.h>

struct Packet;
struct IpAddress;

struct UdpSocket
{
	UdpSocket();
	~UdpSocket();

	bool IsValid() const { return socket != INVALID_SOCKET; }

	int BindTo(const IpAddress&) const;

	bool Send(const Packet& packet, const IpAddress& recipient) const;

	bool CheckPendingPacket(long delay = 500) const;
	bool ReceivePacket(Packet& outPacket, IpAddress& outSender) const;

	operator SOCKET() const { return socket; }
	SOCKET* operator&() { return &socket; }
	const SOCKET* operator&() const { return &socket; }

	SOCKET socket;
};
