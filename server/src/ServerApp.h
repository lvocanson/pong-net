#pragma once
#include "Network/WsaData.h"
#include "Network/UdpSocket.h"
#include "Network/PacketUnwrapper.h"
#include "Network/IpAddress.h"
#include "Utils/Timer.h"

class ServerApp
{
public:

	ServerApp();
	int Run();
	~ServerApp();

private:

	void CheckQuitKeyPressed();
	void CheckPendingPackets();
	void OnPacketReceived(const Packet& packet);
	void OnMessageReceived(const Message& message, const uint16_t sender);

private:

	Timer m_UpTime;

	enum
	{
		Running = 0,
		InitFailed,
		ErrorWhileRunning,
		QuitRequest
	} m_Status;

	WsaData m_WsaData;
	UdpSocket m_Socket;
	IpAddress m_Addr;

	std::vector<PacketUnwrapper> m_Unwrappers;
};
