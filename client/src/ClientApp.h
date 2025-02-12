#pragma once
//#include "Game/Pong.h"
#include "Network/WsaData.h"
#include "Network/UdpSocket.h"
#include "Network/IpAddress.h"
#include "Network/PacketUnwrapper.h"
#include "UI/SceneMachine.h"
#include <SFML/Graphics.hpp>
#include <SFML/Audio.hpp>
#include <swap_back_array.h>

enum class ConnectionStateInfos
{
	None = 0,
	FailedConnection,
	IsConnected,
	IsDisconnected
};

enum class PlayingState
{
	No,
	Paused,
	Playing
};

class ClientApp
{
	using TimePoint = std::chrono::high_resolution_clock::time_point;
public:

	ClientApp();
	int Run();
	~ClientApp();

public: // Getters

	PlayingState GetPlayingState() const { return m_Playing; }

	Pong& GetPongGame() { return m_PongGame; };
	std::tuple<uint16_t, uint16_t> GetScores() { return {m_LeftScore, m_RightScore}; }

	void ConnectToServer(IpPhrase phrase);
	void SendQuickMatchRequest() const;

public: // Methods

	void Quit() { m_Status = QuitRequested; }

private: // Main loop:

	void UpdateUI(float dt);

	void CheckPendingPackets();
	void OnPacketReceived(const Packet& packet);
	void OnMessageReceived(const Message& message);
	void FlushLostPackets(TimePoint now);

	void UpdatePong(float dt);

private: // variables

	enum
	{
		Running = 0,
		InitFailed,
		ErrorWhileRunning,
		QuitRequested,
	} m_Status;

	SceneMachine m_SceneMachine;
	sf::RenderWindow m_Window;
	sf::Font m_Font;
	sf::Music m_Music;

	Pong m_PongGame;
	std::vector<uint16_t> m_RoomIds;
	uint16_t m_LeftScore, m_RightScore;

	PlayingState m_Playing;
	ConnectionStateInfos connectionStateInfo;

private: // server communication

	WsaData m_WsaData;
	UdpSocket m_Socket;
	IpAddress m_ServerAddr;
	uint16_t m_Signature;
	TimePoint m_LastPacketReceivedTp;
	stc::swap_back_array<PacketUnwrapper> m_Unwrappers;
};
