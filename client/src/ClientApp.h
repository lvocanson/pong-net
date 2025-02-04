#pragma once
#include "Game/Pong.h"
#include "Network/NetHelper.h"
#include "PongDisplay.h"
#include "Utils/Timer.h"
#include <SFML/Graphics.hpp>
#include <SFML/Audio.hpp>

class ClientApp
{
public:

	ClientApp();
	int Run();

private:

	void PollEvents();
	void Update(float dt);
	void Display();

private:

	void ConnectToServer(std::string_view address);

private:

	sf::RenderWindow m_Window;
	sf::Font m_Font;
	sf::Music m_Music;

	Pong m_PongGame;
	PongDisplay m_PongDisplay;
	unsigned m_LeftScore, m_RightScore;

	Timer m_Timer;

private: // server communication

	NetHelper::WsaData m_WsaData;
	NetHelper::UdpSocket m_Socket;
	NetHelper::UdpAddress m_ServerAddr;
};
