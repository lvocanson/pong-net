#include "UdpSocket.h"
#include "IpAddress.h"
#include "Packet.h"

UdpSocket::UdpSocket()
	: socket(::socket(AF_INET, SOCK_DGRAM, IPPROTO_UDP))
{
	u_long mode = 1; // Enable non-blocking mode
	ioctlsocket(socket, FIONBIO, &mode);
}

UdpSocket::~UdpSocket()
{
	closesocket(socket);
}

int UdpSocket::BindTo(const IpAddress& addr) const
{
	return bind(socket, &addr, IpAddress::size());
}

bool UdpSocket::Send(const Packet& packet, const IpAddress& recipient) const
{
	return sizeof(Packet) == sendto(socket, reinterpret_cast<const char*>(&packet), sizeof(Packet), 0, &recipient, IpAddress::size());
}

bool UdpSocket::CheckPendingPacket(long delay) const
{
	fd_set readSet;
	FD_ZERO(&readSet);
	FD_SET(socket, &readSet);

	TIMEVAL timeout{};
	timeout.tv_usec = delay; // Microseconds (us)

	int result = select(0, &readSet, NULL, NULL, &timeout);
	if (result == SOCKET_ERROR)
		// TODO: signal error
		return false;

	if (result > 0)
		return true;

	// timeout
	return false;
}

bool UdpSocket::ReceivePacket(Packet& outPacket, IpAddress& outSender) const
{
	int size = IpAddress::size();
	int result = recvfrom(socket, reinterpret_cast<char*>(&outPacket), sizeof(Packet), 0, &outSender, &size);
	return result != SOCKET_ERROR;
}
