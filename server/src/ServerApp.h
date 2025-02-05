#pragma once
#include "ClientDirectory.h"
#include "PongRoom.h"
#include "Network/WsaData.h"
#include "Network/UdpSocket.h"
#include "Network/PacketUnwrapper.h"
#include "Network/IpAddress.h"
#include <swap_back_array.h>
#include <chrono>

class ServerApp
{
	using TimePoint = std::chrono::high_resolution_clock::time_point;
public:


	ServerApp();
	int Run();
	~ServerApp();

private:

	void UpdateStatus();

	void HandlePendingPackets();
	void OnPacketReceived(const Packet& packet, const Client& sender);
	void OnMessageReceived(const Message& message, const Client& sender);
	void FlushLostPackets(TimePoint now);

	void CleanupDirectory(TimePoint now);

	void MaintainRooms(TimePoint now, float dt);

	void LogInfo(std::string_view info) const;
	void LogWarning(std::string_view warning) const;
	void LogError(std::string_view error) const;
	void LogWsaError(std::string_view what, int error = 0) const;

private:


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
	TimePoint m_LastClientsCleanup;

	stc::swap_back_array<PacketUnwrapper> m_Unwrappers;
	stc::swap_back_array<PongRoom> m_PlayingRooms;
	stc::swap_back_array<PongRoom> m_PausedRooms;
};
