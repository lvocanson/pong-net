#include "ClientApp.h"
#include <cstdlib>

ClientApp::ClientApp()
	: m_Window(sf::VideoMode{sf::Vector2u(sf::Vector2{GameSizeX, GameSizeY})}, "Pong")
	, m_Font("res/fonts/JuliaMono-Regular.ttf")
	, m_Music("res/Su Turno.ogg")
	, m_PongGame()
	, m_PongDisplay(m_Font)
	, m_LeftScore(0), m_RightScore(0)
	, m_Timer()

	, m_WsaData()
	, m_Socket()
	, m_ServerAddr(NetHelper::UdpAddress::None)
{
	if (m_WsaData.error || !m_Socket.IsValid())
	{
		// TODO: error handling
		m_Window.close();
		return;
	}

	m_Music.play();
	m_Music.setLooping(true);
}

int ClientApp::Run()
{
	if (!m_Window.isOpen())
	{
		return EXIT_FAILURE;
	}

	// TODO: call from UI
	ConnectToServer("127.0.0.1");

	Timer dtTimer;
	do
	{
		PollEvents();

		float dt = dtTimer.GetElapsedSeconds();
		dtTimer.Restart();
		Update(dt);

		Display();

	} while (m_Window.isOpen());

	return EXIT_SUCCESS;
}

static PaddlesBehaviour operator|=(PaddlesBehaviour& lhs, PaddlesBehaviour rhs)
{
	using UType = std::underlying_type_t<PaddlesBehaviour>;
	return lhs = static_cast<PaddlesBehaviour>(static_cast<UType>(lhs) | static_cast<UType>(rhs));
}

static PaddlesBehaviour operator&=(PaddlesBehaviour& lhs, PaddlesBehaviour rhs)
{
	using UType = std::underlying_type_t<PaddlesBehaviour>;
	return lhs = static_cast<PaddlesBehaviour>(static_cast<UType>(lhs) & static_cast<UType>(rhs));
}

static PaddlesBehaviour operator~(PaddlesBehaviour rhs)
{
	using UType = std::underlying_type_t<PaddlesBehaviour>;
	return static_cast<PaddlesBehaviour>(~static_cast<UType>(rhs));
}

void ClientApp::PollEvents()
{
	while (const std::optional event = m_Window.pollEvent())
	{
		if (event->is<sf::Event::Closed>())
			m_Window.close();

		if (auto* keyEvent = event->getIf<sf::Event::KeyPressed>())
		{
			using enum sf::Keyboard::Key;
			using enum PaddlesBehaviour;
			switch (keyEvent->code)
			{
			case W:
			case Z:
				m_PongGame.Behaviours |= LeftUp;
				break;
			case S:
				m_PongGame.Behaviours |= LeftDown;
				break;
			case Up:
				m_PongGame.Behaviours |= RightUp;
				break;
			case Down:
				m_PongGame.Behaviours |= RightDown;
				break;
			}
		}

		if (auto* keyEvent = event->getIf<sf::Event::KeyReleased>())
		{
			using enum sf::Keyboard::Key;
			using enum PaddlesBehaviour;
			switch (keyEvent->code)
			{
			case W:
			case Z:
				m_PongGame.Behaviours &= ~LeftUp;
				break;
			case S:
				m_PongGame.Behaviours &= ~LeftDown;
				break;
			case Up:
				m_PongGame.Behaviours &= ~RightUp;
				break;
			case Down:
				m_PongGame.Behaviours &= ~RightDown;
				break;
			}
		}
	}
}

void ClientApp::Update(float dt)
{
	m_PongGame.Update(dt);
	m_PongDisplay.Update(m_PongGame);

	switch (m_PongGame.GetGameState())
	{
	case GameState::LeftWins:
	{
		++m_LeftScore;
		break;
	}
	case GameState::RightWins:
	{
		++m_RightScore;
		break;
	}
	default: return;
	}

	m_PongGame.Reset();
	m_PongDisplay.SetScore(m_LeftScore, m_RightScore);
}

void ClientApp::Display()
{
	m_Window.clear();
	m_PongDisplay.Draw(m_Window);
	m_Window.display();
}

void ClientApp::ConnectToServer(std::string_view address)
{
	m_ServerAddr = NetHelper::UdpAddress(address.data());

	// TODO: encapsulate
	int msgLength = 4;
	int bytesSent = sendto(m_Socket, "Test", msgLength, 0, &m_ServerAddr, NetHelper::UdpAddress::size());
	if (bytesSent == SOCKET_ERROR)
	{
		// TODO: error handling
		m_Window.close();
	}
	if (bytesSent != msgLength)
	{
		// TODO: error handling
		m_Window.close();
	}
}
