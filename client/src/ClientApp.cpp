#define NOMINMAX
#include "ClientApp.h"
#include "Network/NetHelper.h"
#include "Network/PacketWrapper.h"
#include "Utils/EnumOperators.h"
#include "Utils/Timer.h"
#include "UI/Scenes/MainMenu.h"
#include "UI/Scenes/GameScene.h"
#include "UserData.h"
#include <cstdlib>

using namespace std::chrono_literals;
inline constexpr auto TimeForLostPacket = 300ms;
inline constexpr sf::Vector2u ScreenSize = sf::Vector2u{sf::Vector2(GameSizeX, GameSizeY)};

static const std::filesystem::path VolumeFilename = "volume.dat";

ClientApp::ClientApp()
	: m_Status(Running)

	, m_Window(sf::VideoMode(ScreenSize), "Pong")
	, m_Font("res/fonts/JuliaMono-Regular.ttf")
	, m_Music("res/Su Turno.ogg")

	, m_PongGame()
	, connectionStateInfo(ConnectionStateInfos::None)
	, m_LeftScore(0)
	, m_RightScore(0)
	, m_Playing(PlayingState::No)

	, m_WsaData()
	, m_Socket()
	, m_ServerAddr()
	, m_Signature(0)
	, m_LastPacketReceivedTp(TimePoint::min())
{
	if (!m_Window.isOpen())
	{
		m_Status = InitFailed;
		return;
	}

	if (float volume; UserData::Load(VolumeFilename, volume))
	{
		m_Music.setVolume(volume);
	}

	m_Music.play();
	m_Music.setLooping(true);

	if (m_WsaData.error || !m_Socket.IsValid())
	{
		m_Status = InitFailed;
		return;
	}

	m_SceneMachine.SetFirstScene<MainMenu>(*this, m_Font, ScreenSize);
}

int ClientApp::Run()
{
	if (m_Status != Running)
	{
		return EXIT_FAILURE;
	}

	Timer dtTimer;
	do
	{
		auto now = std::chrono::high_resolution_clock::now();
		float dt = dtTimer.GetElapsedSeconds();
		dtTimer.Restart();

		UpdateUI(dt);

		CheckPendingPackets();
		FlushLostPackets(now);

		UpdatePong(dt);

	} while (m_Status == Running);

	return EXIT_SUCCESS;
}

ClientApp::~ClientApp()
{
	UserData::Save(VolumeFilename, m_Music.getVolume());
	m_Music.stop();
	m_Window.close();
}

void ClientApp::UpdateUI(float dt)
{
	while (const std::optional event = m_Window.pollEvent())
	{
		if (event->is<sf::Event::Closed>())
			Quit();

		if (auto data = event->getIf<sf::Event::Resized>())
		{
			sf::View view = m_Window.getView();
			auto sizef = sf::Vector2f(data->size);
			view.setSize(sizef);
			view.setCenter(sizef / 2.f);
			m_Window.setView(view);
		}

		m_SceneMachine.OnEvent(event.value());
	}

	m_SceneMachine.Update(dt);

	m_Window.clear({51, 56, 63});
	m_SceneMachine.Draw(m_Window);
	m_Window.display();
}

void ClientApp::ConnectToServer(IpPhrase phrase)
{
	m_ServerAddr = phrase;

	Message connect(MessageType::Connect);
	auto wrapper = PacketWrapper::Wrap(connect);
	wrapper.Sign(m_Signature);
	if (!wrapper.Send(m_Socket, m_ServerAddr))
	{
		std::string_view error = NetHelper::GetWsaErrorExplanation();
		// TODO: print error
		connectionStateInfo = ConnectionStateInfos::FailedConnection;
	}
}

void ClientApp::SendQuickMatchRequest() const
{
	Message request(MessageType::QuickMatchRequest);
	auto wrapper = PacketWrapper::Wrap(request);
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
				m_LastPacketReceivedTp = packet.header.timestamp;
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
			it = m_Unwrappers.erase_swap(it);
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

		// TODO: make a menu instead
		SendQuickMatchRequest();
	}
	break;
	case RoomGroupResponse:
	{
		auto& response = message.As<Message_RoomGroupResponse>();
		if (response.grpIdx > 0)
		{
			// TODO: enable "prev page" button
		}
		if (response.grpIdx < response.maxGrpIdx)
		{
			// TODO: enable "next page" button
		}

		for (auto uuid : response.group)
		{
			if (uuid == 0)
				// End of the group
				break;

			m_RoomIds.emplace_back(uuid);
		}
	}
	break;
	case RoomJoinResponse:
	{
		auto& response = message.As<Message_RoomJoinResponse>();

		switch (response.status)
		{
		case Message_RoomJoinResponse::Rejected:
		{
			// TODO: notify reject
		}
		break;
		case Message_RoomJoinResponse::Accepted:
		{
			m_SceneMachine.ChangeScene<GameScene>(*this, m_Font, m_Window.getSize());
		}
		break;
		}
	}
	break;
	case GameUpdate:
	{
		auto& update = message.As<Message_GameUpdate>();

		TimePoint now = std::chrono::high_resolution_clock::now();
		float diff = std::chrono::duration<float>(now - m_LastPacketReceivedTp).count();

		switch (update.status)
		{
		case Message_GameUpdate::Paused:
		{
			m_Playing = PlayingState::Paused;
		}
		break;
		case Message_GameUpdate::Playing:
		{
			m_Playing = PlayingState::Playing;

			auto tempGame = update.game;
			tempGame.Behaviours = m_PongGame.Behaviours;
			tempGame.Update(diff);

			// Make up for lost time
			using enum PaddlesBehaviour;
			if ((m_PongGame.Behaviours & LeftUp) != None)
				tempGame.LeftPaddle = std::min(tempGame.LeftPaddle, m_PongGame.LeftPaddle);
			else if ((m_PongGame.Behaviours & LeftDown) != None)
				tempGame.LeftPaddle = std::max(tempGame.LeftPaddle, m_PongGame.LeftPaddle);
			if ((m_PongGame.Behaviours & RightUp) != None)
				tempGame.RightPaddle = std::min(tempGame.RightPaddle, m_PongGame.RightPaddle);
			else if ((m_PongGame.Behaviours & RightDown) != None)
				tempGame.RightPaddle = std::max(tempGame.RightPaddle, m_PongGame.RightPaddle);

			m_PongGame = tempGame;
			m_LeftScore = update.leftScore;
			m_RightScore = update.rightScore;

		}
		break;
		}
	}
	break;
	}
}

void ClientApp::UpdatePong(float dt)
{
	if (m_Playing == PlayingState::Playing)
	{
		m_PongGame.Update(dt);
	}

	if (m_Playing != PlayingState::No)
	{
		Message_GameUpdate update(m_PongGame, m_Playing == PlayingState::No ? Message_GameUpdate::Playing : Message_GameUpdate::Paused, m_LeftScore, m_RightScore);
		auto wrapper = PacketWrapper::Wrap(update);
		wrapper.Sign(m_Signature);
		wrapper.Send(m_Socket, m_ServerAddr);
	}
}

void ClientApp::FlushLostPackets(TimePoint now)
{
	for (auto it = m_Unwrappers.begin(); it != m_Unwrappers.end();)
	{
		if (now - it->Timestamp() > TimeForLostPacket)
		{
			it = m_Unwrappers.erase_swap(it);
			continue;
		}
		++it;
	}
}
