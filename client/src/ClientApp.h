#pragma once
#include "Game/Pong.h"
#include "Network/WsaData.h"
#include "Network/UdpSocket.h"
#include "Network/IpAddress.h"
#include "PongDisplay.h"
#include "Utils/Timer.h"
#include <SFML/Graphics.hpp>
#include <SFML/Audio.hpp>
#include "StateMachine/StateMachine.h"

class Window;
class InputHandler;

class ClientApp : public StateMachine<ClientApp>
{
public:

	ClientApp();
	int Run();
	void Shutdown() { m_Window->Close(); }
	void ConnectToServer(std::string_view address);

	Window* GetWindow();
	sf::Music* GetMusic();
	InputHandler* GetInputHandler();

private:

	void PollEvents();
	void Update(float dt);

private: // variables

	Window* m_Window = nullptr;
	sf::Music* m_Music;
	InputHandler* m_inputHandler;
	Pong m_PongGame;
	PongDisplay* m_PongDisplay;
	unsigned m_LeftScore, m_RightScore;

	Timer m_Timer;

private: // server communication

	WsaData m_WsaData;
	UdpSocket m_Socket;
	IpAddress m_ServerAddr;
	uint16_t m_Signature;
};
