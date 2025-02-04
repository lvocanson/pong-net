#pragma once
#include "Network/NetHelper.h"
#include "Utils/Timer.h"

class ServerApp
{
public:

	ServerApp();
	int Run();
	~ServerApp();

private:

	bool IsQuitKeyPressed();
	void OnMessageReceived(NetHeader& header, NetMessage<NetMessageType::Unknown>& message, NetHelper::UdpAddress& sender);

private:

	Timer m_UpTime;

	enum
	{
		Running = 0,
		InitFailed,
		ErrorWhileRunning,
		QuitRequest
	} m_Status;

	NetHelper::WsaData m_WsaData;
	NetHelper::UdpSocket m_Socket;
	NetHelper::UdpAddress m_Addr;
};
