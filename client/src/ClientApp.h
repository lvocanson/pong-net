#pragma once
#include "Game/Pong.h"
#include "Network/NetHelper.h"
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

	Window* GetWindow();
	InputHandler* GetInputHandler();

private:

	void PollEvents();
	void Update(float dt);

private:

	void ConnectToServer(std::string_view address);

private:

	Window* m_Window = nullptr;
	sf::Music m_Music;
	InputHandler* m_inputHandler;
	Pong m_PongGame;
	PongDisplay* m_PongDisplay;
	unsigned m_LeftScore, m_RightScore;

	Timer m_Timer;

private: // server communication

	NetHelper::WsaData m_WsaData;
	NetHelper::UdpSocket m_Socket;
	NetHelper::UdpAddress m_ServerAddr;
};
