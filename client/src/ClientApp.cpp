#include "ClientApp.h"

#include <cstdlib>

#include "Window/Window.h"
#include "../FontRegistry.h"

#include <StateMachine/StateMachine.h>
#include "StateMachine/AppState/MenuState.h"
#include "StateMachine/AppState/ConnectionState.h"
#include <Window/InputHandler.h>

ClientApp::ClientApp()
	: m_Music("res/Su Turno.ogg")
	, m_PongGame()
	, m_PongDisplay(nullptr)
	, m_LeftScore(0)
	, m_RightScore(0)
	, m_Timer()
	, m_WsaData()
	, m_Socket()
	, m_ServerAddr(NetHelper::UdpAddress::None)
{
	m_Window = new Window();
	m_Window->Create("Pong", GameSizeX, GameSizeY);

	FontRegistry::LoadFont("JuliaMono-Regular.ttf");
	m_PongDisplay = new PongDisplay(*FontRegistry::GetFont("JuliaMono-Regular.ttf"));	

	m_inputHandler = new InputHandler();
	
	SetFirstState<MenuState>();
	/*changedstat
	{
		m_StateMachine->AddState("MenuState", new MenuState());
		m_StateMachine->AddState("ConnectionState", new ConnectionState(m_StateMachine, m_Window));
		m_StateMachine->InitState("MenuState");
		m_StateMachine->Start();
	}*/

	if (m_WsaData.error || !m_Socket.IsValid())
	{
		// TODO: error handling
		//m_Window.close();
		return;
	}

	m_Music.play();
	m_Music.setLooping(true);
}

int ClientApp::Run()
{
	if (!m_Window->IsOpen())
	{
		return EXIT_FAILURE;
	}

	// TODO: call from UI
	ConnectToServer("127.0.0.1");

	Timer dtTimer;
	while (m_Window->IsOpen())
	{
		PollEvents();
		m_inputHandler->Update();

		float dt = dtTimer.GetElapsedSeconds();
		dtTimer.Restart();
		Update(dt);

		m_Window->Render();
	}

	return EXIT_SUCCESS;
}

Window* ClientApp::GetWindow()
{
	return m_Window;
}

InputHandler* ClientApp::GetInputHandler()
{
	return m_inputHandler;
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
	std::function<void(sf::Keyboard::Key)> onKeyPressed = [this](sf::Keyboard::Key code)
		{
			using enum sf::Keyboard::Key;
			using enum PaddlesBehaviour;

			switch (code)
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
		};

	std::function<void(sf::Keyboard::Key)> onKeyReleased = [this](sf::Keyboard::Key code)
		{
			using enum sf::Keyboard::Key;
			using enum PaddlesBehaviour;
			switch (code)
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
		};

	m_Window->PollEvents(onKeyPressed, onKeyReleased);
}

void ClientApp::Update(float dt)
{
	StateMachine::Update(dt);

	/*m_PongGame.Update(dt);
	m_PongDisplay->Update(m_PongGame);

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
	m_PongDisplay->SetScore(m_LeftScore, m_RightScore);*/
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
		//m_Window.close();
	}
	if (bytesSent != msgLength)
	{
		// TODO: error handling
		//m_Window.close();
	}
}
