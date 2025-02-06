#pragma once
//#include "Game/Pong.h"
#include "Network/WsaData.h"
#include "Network/UdpSocket.h"
#include "Network/IpAddress.h"
#include "Network/PacketUnwrapper.h"
#include "PongDisplay.h"
#include "StateMachine/StateMachine.h"
#include "Utils/Timer.h"
#include <SFML/Graphics.hpp>
#include <SFML/Audio.hpp>
#include <swap_back_array.h>

class Window;
class InputHandler;
class FontRegistry;

enum class ConnectionStateInfos
{
	None = 0,
	FailedConnection,
	IsConnected,
	IsDisconnected
};

class ClientApp : public StateMachine<ClientApp>
{
public:

	ClientApp();
	int Run();
	void Shutdown();

	Window* GetWindow();
	sf::Music* GetMusic();
	InputHandler* GetInputHandler();
	sf::Font* GetFontByName(const std::string& fontName);
	ConnectionStateInfos GetConnectionStateInfo() { return connectionStateInfo; };

	void ConnectToServer(std::string_view address);

private:

	void PollEvents();
	void Update(float dt);

	void CheckPendingPackets();
	void OnPacketReceived(const Packet& packet);
	void OnMessageReceived(const Message& message);

private: // variables

	Window* m_Window = nullptr;
	sf::Music* m_Music;
	InputHandler* m_inputHandler;
	FontRegistry* m_font;

	Timer m_Timer;
	ConnectionStateInfos connectionStateInfo;

private: // server communication

	WsaData m_WsaData;
	UdpSocket m_Socket;
	IpAddress m_ServerAddr;
	uint16_t m_Signature;
	stc::swap_back_array<PacketUnwrapper> m_Unwrappers;
};
