#pragma once
//#include "Game/Pong.h"
#include "Network/WsaData.h"
#include "Network/UdpSocket.h"
#include "Network/IpAddress.h"
#include "Network/PacketUnwrapper.h"
#include "PongDisplay.h"
#include "StateMachine/StateMachine.h"
#include "Utils/Timer.h"
#include "Window/InputHandler.h"
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

class ClientApp : public StateMachine<ClientApp>
{
	using TimePoint = std::chrono::high_resolution_clock::time_point;
public:

	ClientApp();
	int Run();
	void Shutdown();

public: // Getters

	PlayingState& GetPlayingState() { return m_Playing; }
	UdpSocket& GetSocket() { return m_Socket; }
	IpAddress& GetServerAddr() { return m_ServerAddr; }
	uint16_t& GetSignature() { return m_Signature; }
	const std::vector<uint16_t>& GetRoomIds() { return m_RoomIds; }
	void ResetRoomIds() { m_RoomIds.clear(); }
	Window& GetWindow() { return m_Window; }
	sf::Music& GetMusic() { return m_Music; }
	const sf::Font& GetFont() const { return m_Font; }
	const InputHandler& GetInputHandler() { return m_Input; }

	Pong& GetPongGame() { return m_PongGame; };
	std::tuple<uint16_t, uint16_t> GetScores() { return {m_LeftScore, m_RightScore}; }

	void ConnectToServer(IpPhrase phrase);
	ConnectionStateInfos GetConnectionStateInfo() const { return connectionStateInfo; };

private:

	void PollEvents();
	void Update(float dt);

	void CheckPendingPackets();
	void OnPacketReceived(const Packet& packet);
	void OnMessageReceived(const Message& message);
	void FlushLostPackets(TimePoint now);

private: // variables

	Window m_Window;
	sf::Font m_Font;
	sf::Music m_Music;
	InputHandler m_Input;

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
