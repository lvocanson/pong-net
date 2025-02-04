#include "NetHelper.h"

namespace NetHelper
{

WsaData::WsaData()
	: data{}
	, error(WSAStartup(MAKEWORD(2, 2), &data))
{
}

WsaData::~WsaData()
{
	if (!error)
		WSACleanup();
}

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

int UdpSocket::BindTo(const UdpAddress& addr) const
{
	return bind(socket, &addr, UdpAddress::size());
}

bool UdpSocket::Send(const char* data, int size, const UdpAddress& recipient) const
{
	return size == sendto(socket, data, size, 0, &recipient, NetHelper::UdpAddress::size());
}

bool UdpSocket::CheckPendingMessage(long delay) const
{
	fd_set readSet;
	FD_ZERO(&readSet);
	FD_SET(socket, &readSet);

	TIMEVAL timeout{};
	timeout.tv_usec = delay; // Microsecondes

	int result = select(0, &readSet, NULL, NULL, &timeout);
	if (result == SOCKET_ERROR)
		// TODO: signal error
		return false;

	if (result > 0)
		return true;

	// timeout
	return false;
}

bool UdpSocket::ReceiveMessage(NetHeader& outHeader, NetMessage<NetMessageType::Unknown>& outMessage, UdpAddress& outSender) const
{
	int size = NetHelper::UdpAddress::size();
	int result = recvfrom(socket, reinterpret_cast<char*>(&outHeader), sizeof(NetHeader), 0, &outSender, &size);

	if (result != sizeof(NetHeader))
		return false;

	if (outHeader.size == 0)
		return true;

	result = recv(socket, outMessage.GetBufferOfSize(outHeader.size), outHeader.size, 0);

	if (result != outHeader.size)
		return false;
	
	return true;
}

inline constexpr u_short DefaultPort = 9551;

UdpAddress::UdpAddress(const char* address)
	: addr
	{
		.sin_family = AF_INET,
		.sin_port = htons(DefaultPort),
	}
{
	inet_pton(addr.sin_family, address, &addr.sin_addr.s_addr);
}

UdpAddress::UdpAddress(AddrSpecialType address)
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

std::string_view GetWsaErrorExplanation(int error)
{
	if (error == 0)
		error = WSAGetLastError();

	// Documentation at https://learn.microsoft.com/en-us/windows/win32/winsock/windows-sockets-error-codes-2

	switch (error)
	{
	case WSA_INVALID_HANDLE: return "Specified event object handle is invalid.";
	case WSA_NOT_ENOUGH_MEMORY: return "Insufficient memory available.";
	case WSA_INVALID_PARAMETER: return "One or more parameters are invalid.";
	case WSA_OPERATION_ABORTED: return "Overlapped operation aborted.";
	case WSA_IO_INCOMPLETE: return "Overlapped I/O event object not in signaled state.";
	case WSA_IO_PENDING: return "Overlapped operations will complete later.";
	case WSAEINTR: return "Interrupted function call.";
	case WSAEBADF: return "File handle is not valid.";
	case WSAEACCES: return "Permission denied.";
	case WSAEFAULT: return "Bad address.";
	case WSAEINVAL: return "Invalid argument.";
	case WSAEMFILE: return "Too many open files.";
	case WSAEWOULDBLOCK: return "Resource temporarily unavailable.";
	case WSAEINPROGRESS: return "Operation now in progress.";
	case WSAEALREADY: return "Operation already in progress.";
	case WSAENOTSOCK: return "UdpSocket operation on nonsocket.";
	case WSAEDESTADDRREQ: return "Destination address required.";
	case WSAEMSGSIZE: return "Message too long.";
	case WSAEPROTOTYPE: return "Protocol wrong type for socket.";
	case WSAENOPROTOOPT: return "Bad protocol option.";
	case WSAEPROTONOSUPPORT: return "Protocol not supported.";
	case WSAESOCKTNOSUPPORT: return "UdpSocket type not supported.";
	case WSAEOPNOTSUPP: return "Operation not supported.";
	case WSAEPFNOSUPPORT: return "Protocol family not supported.";
	case WSAEAFNOSUPPORT: return "Address family not supported by protocol family.";
	case WSAEADDRINUSE: return "Address already in use.";
	case WSAEADDRNOTAVAIL: return "Cannot assign requested address.";
	case WSAENETDOWN: return "Network is down.";
	case WSAENETUNREACH: return "Network is unreachable.";
	case WSAENETRESET: return "Network dropped connection on reset.";
	case WSAECONNABORTED: return "Software caused connection abort.";
	case WSAECONNRESET: return "Connection reset by peer.";
	case WSAENOBUFS: return "No buffer space available.";
	case WSAEISCONN: return "UdpSocket is already connected.";
	case WSAENOTCONN: return "UdpSocket is not connected.";
	case WSAESHUTDOWN: return "Cannot send after socket shutdown.";
	case WSAETOOMANYREFS: return "Too many references.";
	case WSAETIMEDOUT: return "Connection timed out.";
	case WSAECONNREFUSED: return "Connection refused.";
	case WSAELOOP: return "Cannot translate name.";
	case WSAENAMETOOLONG: return "Name too long.";
	case WSAEHOSTDOWN: return "Host is down.";
	case WSAEHOSTUNREACH: return "No route to host.";
	case WSAENOTEMPTY: return "Directory not empty.";
	case WSAEPROCLIM: return "Too many processes.";
	case WSAEUSERS: return "User quota exceeded.";
	case WSAEDQUOT: return "Disk quota exceeded.";
	case WSAESTALE: return "Stale file handle reference.";
	case WSAEREMOTE: return "Item is remote.";
	case WSASYSNOTREADY: return "Network subsystem is unavailable.";
	case WSAVERNOTSUPPORTED: return "Winsock.dll version out of range.";
	case WSANOTINITIALISED: return "Successful WSAStartup not yet performed.";
	case WSAEDISCON: return "Graceful shutdown in progress.";
	case WSAENOMORE: return "No more results.";
	case WSAECANCELLED: return "Call has been canceled.";
	case WSAEINVALIDPROCTABLE: return "Procedure call table is invalid.";
	case WSAEINVALIDPROVIDER: return "Service provider is invalid.";
	case WSAEPROVIDERFAILEDINIT: return "Service provider failed to initialize.";
	case WSASYSCALLFAILURE: return "System call failure.";
	case WSASERVICE_NOT_FOUND: return "Service not found.";
	case WSATYPE_NOT_FOUND: return "Class type not found.";
	case WSA_E_NO_MORE: return "No more results.";
	case WSA_E_CANCELLED: return "Call was canceled.";
	case WSAEREFUSED: return "Database query was refused.";
	case WSAHOST_NOT_FOUND: return "Host not found.";
	case WSATRY_AGAIN: return "Nonauthoritative host not found.";
	case WSANO_RECOVERY: return "This is a nonrecoverable error.";
	case WSANO_DATA: return "Valid name, no data record of requested type.";
	case WSA_QOS_RECEIVERS: return "QoS receivers.";
	case WSA_QOS_SENDERS: return "QoS senders.";
	case WSA_QOS_NO_SENDERS: return "No QoS senders.";
	case WSA_QOS_NO_RECEIVERS: return "QoS no receivers.";
	case WSA_QOS_REQUEST_CONFIRMED: return "QoS request confirmed.";
	case WSA_QOS_ADMISSION_FAILURE: return "QoS admission error.";
	case WSA_QOS_POLICY_FAILURE: return "QoS policy failure.";
	case WSA_QOS_BAD_STYLE: return "QoS bad style.";
	case WSA_QOS_BAD_OBJECT: return "QoS bad object.";
	case WSA_QOS_TRAFFIC_CTRL_ERROR: return "QoS traffic control error.";
	case WSA_QOS_GENERIC_ERROR: return "QoS generic error.";
	case WSA_QOS_ESERVICETYPE: return "QoS service type error.";
	case WSA_QOS_EFLOWSPEC: return "QoS flowspec error.";
	case WSA_QOS_EPROVSPECBUF: return "Invalid QoS provider buffer.";
	case WSA_QOS_EFILTERSTYLE: return "Invalid QoS filter style.";
	case WSA_QOS_EFILTERTYPE: return "Invalid QoS filter type.";
	case WSA_QOS_EFILTERCOUNT: return "Incorrect QoS filter count.";
	case WSA_QOS_EOBJLENGTH: return "Invalid QoS object length.";
	case WSA_QOS_EFLOWCOUNT: return "Incorrect QoS flow count.";
	case WSA_QOS_EUNKOWNPSOBJ: return "Unrecognized QoS object.";
	case WSA_QOS_EPOLICYOBJ: return "Invalid QoS policy object.";
	case WSA_QOS_EFLOWDESC: return "Invalid QoS flow descriptor.";
	case WSA_QOS_EPSFLOWSPEC: return "Invalid QoS provider-specific flowspec.";
	case WSA_QOS_EPSFILTERSPEC: return "Invalid QoS provider-specific filterspec.";
	case WSA_QOS_ESDMODEOBJ: return "Invalid QoS shape discard mode object.";
	case WSA_QOS_ESHAPERATEOBJ: return "Invalid QoS shaping rate object.";
	case WSA_QOS_RESERVED_PETYPE: return "Reserved policy QoS element type.";
	}

	return "Unknown error.";
}

} // namespace NetworkHelper
