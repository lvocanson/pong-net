#pragma once
#include "ClientDirectory.h"
#include "Network/WsaData.h"
#include "Network/UdpSocket.h"
#include "Network/PacketUnwrapper.h"
#include "Network/IpAddress.h"
#include "Utils/Timer.h"
#include <swap_back_array.h>

class ServerApp
{
public:

	ServerApp();
	int Run();
	~ServerApp();

private:

	void UpdateStatus();

	void HandlePendingPackets();
	void OnPacketReceived(const Packet& packet);
	void OnMessageReceived(const Message& message, const uint16_t sender);

	void LogInfo(std::string_view info) const;
	void LogWarning(std::string_view warning) const;
	void LogError(std::string_view error) const;
	void LogWsaError(std::string_view what, int error = 0) const;

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

	ClientDirectory m_Clients;
	stc::swap_back_array<PacketUnwrapper> m_Unwrappers;
	stc::swap_back_array<PongRoom> m_PlayingRooms;
	stc::swap_back_array<PongRoom> m_PausedRooms;
};
