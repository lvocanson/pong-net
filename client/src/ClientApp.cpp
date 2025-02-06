#include "ClientApp.h"
#include "Network/NetHelper.h"
#include "Network/PacketWrapper.h"
#include <cstdlib>

#include "Window/Window.h"
#include "FontRegistry.h"

#include "StateMachine/StateMachine.h"
#include "StateMachine/AppState/MenuState.h"
#include "StateMachine/AppState/ConnectionState.h"
#include "Window/InputHandler.h"

ClientApp::ClientApp()
	: m_Timer()
	, m_WsaData()
	, m_Socket()
	, m_ServerAddr()
	, m_Signature(0)
{
	m_Window = new Window();
	m_Window->Create("Pong", sf::Vector2u(sf::Vector2{GameSizeX, GameSizeY}));

	m_inputHandler = new InputHandler();

	m_Music = new sf::Music("res/Su Turno.ogg");

	m_font = new FontRegistry();
	m_font->LoadFont("JuliaMono-Regular.ttf");

	SetFirstState<MenuState>();

	if (m_WsaData.error || !m_Socket.IsValid())
	{
		// TODO: error handling
		//m_Window.close();
		return;
	}

	m_Music->play();
	m_Music->setLooping(true);
}

int ClientApp::Run()
{
	if (!m_Window->IsOpen())
	{
		return EXIT_FAILURE;
	}

	Timer dtTimer;
	do
	{
		PollEvents();
		m_inputHandler->Update();

		CheckPendingPackets();

		float dt = dtTimer.GetElapsedSeconds();
		dtTimer.Restart();
		Update(dt);

		m_Window->Render();
	} while (m_Window->IsOpen());

	return EXIT_SUCCESS;
}

void ClientApp::Shutdown()
{
	m_Window->Close();
}

Window* ClientApp::GetWindow()
{
	return m_Window;
}

sf::Music* ClientApp::GetMusic()
{
	return m_Music;
}

InputHandler* ClientApp::GetInputHandler()
{
	return m_inputHandler;
}

sf::Font* ClientApp::GetFontByName(const std::string& fontName)
{
	return m_font->GetFont(fontName);
}

void ClientApp::PollEvents()
{
	m_Window->PollEvents();
}

void ClientApp::Update(float dt)
{
	StateMachine::Update(dt);
}

void ClientApp::ConnectToServer(std::string_view address)
{
	m_ServerAddr = address.data();

	Message_Connect connect;
	auto wrapper = PacketWrapper::Wrap(connect);
	wrapper.Sign(m_Signature);
	if (!wrapper.Send(m_Socket, m_ServerAddr))
	{
		std::string_view error = NetHelper::GetWsaErrorExplanation();
		// TODO: print error
	}
}

void ClientApp::CheckPendingPackets()
{
	while (m_Socket.CheckPendingPacket(1))
	{
		Packet packet;
		IpAddress sender;
		if (m_Socket.ReceivePacket(packet, sender))
		{
			if (sender != m_ServerAddr)
			{
				// ignore
			}
			else
			{
				OnPacketReceived(packet);
			}
		}
		else
		{
			// silent fail
			NetHelper::GetWsaErrorExplanation();
		}
	}
}

void ClientApp::OnPacketReceived(const Packet& packet)
{
	if (!packet.IsValid())
	{
		// silent fail
		return;
	}

	for (auto it = m_Unwrappers.begin(); it != m_Unwrappers.end(); ++it)
	{
		if (it->TryAddPacket(packet) && it->IsComplete())
		{
			auto& message = it->Unwrap<Message>();
			OnMessageReceived(message);
			m_Unwrappers.erase_swap(it);
			return;
		}
	}

	PacketUnwrapper unwrapper(packet);
	if (unwrapper.IsComplete())
	{
		auto& message = unwrapper.Unwrap<Message>();
		OnMessageReceived(message);
	}
	else
	{
		m_Unwrappers.emplace_back(std::move(unwrapper));
	}
}

void ClientApp::OnMessageReceived(const Message& message)
{
	using enum MessageType;
	switch (message.type)
	{
	case ConnectResponse:
	{
		auto& response = message.As<Message_ConnectResponse>();
		m_Signature = response.signature;
		// TODO: signal connect success
	}
	break;
	}
}
