#include "ServerApp.h"
#include "Network/NetHelper.h"
#include "Network/PacketWrapper.h"
#include "Utils/Console.h"
#include "Utils/Misc.h"
#include "Utils/Timer.h"
#include <thread>

using namespace std::chrono_literals;
inline constexpr auto ClientDirectoryCleanupInterval = 5min;
inline constexpr auto ClientDirectoryMaxLastContact = 10min;
inline constexpr auto InactivityUpdateMissingTime = 1s;
inline constexpr auto TimeForLostPacket = 300ms;

ServerApp::ServerApp()
	: m_Status(Running)
	, m_WsaData()
	, m_Socket()
	, m_Addr(IpAddress::Any)
	, m_LastClientsCleanup(std::chrono::high_resolution_clock::now())
{
	int error;

	using namespace Console;
	Out << WindowTitle::SetTitle("Pong Server Console");

	if (m_WsaData.error)
	{
		LogWsaError("WSA initialisation failed", m_WsaData.error);
		goto init_failed;
	}
	LogInfo("WSA initialisation OK");

	if (!m_Socket.IsValid())
	{
		LogWsaError("Socket creation failed");
		goto init_failed;
	}
	LogInfo("Socket creation OK");

	error = m_Socket.BindTo(m_Addr);
	if (error)
	{
		LogWsaError("Socket bind failed", error);
		goto init_failed;
	}
	LogInfo("Socket bind OK");

	LogInfo("Initialization successful");
	return;

init_failed:

	m_Status = InitFailed;
	LogError("Initialization failed");
}

int ServerApp::Run()
{
	if (m_Status != Running)
		return EXIT_FAILURE;

	using namespace Console;
	Out << TextColors::BrightFgBlack
		<< "The server is running at port "
		<< TextColors::BrightFgMagenta
		<< m_Addr.addr.sin_port
		<< TextColors::BrightFgBlack
		<< ".\nPress ESC to shutdown.\n"
		<< TextColors::Reset;

	Timer dtTimer;
	do
	{
		auto now = std::chrono::high_resolution_clock::now();
		float dt = dtTimer.GetElapsedSeconds();
		dtTimer.Restart();

		UpdateStatus();

		HandlePendingPackets();
		FlushLostPackets(now);

		CleanupDirectory(now);

		MaintainRooms(now, dt);

		// Sleep for 1ms to avoid 100% CPU usage
		using namespace std::chrono_literals;
		std::this_thread::sleep_for(1ms);
	} while (m_Status == Running);

	return EXIT_SUCCESS;
}

ServerApp::~ServerApp()
{
	using namespace Console;
	LogInfo("The server is shuting down");

	std::string_view reason = "Reason: Unknown";
	switch (m_Status)
	{
	case InitFailed:
		reason = "Reason: Initialization failed";
		break;
	case ErrorWhileRunning:
		reason = "Reason: Error while running";
		break;
	case QuitRequest:
		reason = "Reason: Quit requested";
		break;
	}
	LogInfo(reason);

	LogInfo("Shutdown successful");
}

#include <conio.h>
void ServerApp::UpdateStatus()
{
	constexpr auto EscapeKey = '\033';
	while (_kbhit())
	{
		if (_getch() == EscapeKey)
		{
			LogInfo("Server quit key pressed");
			m_Status = QuitRequest;
		}
	}
}

void ServerApp::HandlePendingPackets()
{
	while (m_Socket.CheckPendingPacket(1))
	{
		Packet packet;
		IpAddress sender;
		if (m_Socket.ReceivePacket(packet, sender))
		{
			auto& client = m_Clients.AddOrUpdate({
				.signature = packet.header.signature,
				.address = sender});
			OnPacketReceived(packet, client);
		}
		else
		{
			LogWsaError("Error while receiving packet");
		}
	}
}

void ServerApp::OnPacketReceived(const Packet& packet, const Client& sender)
{
	if (!packet.IsValid())
	{
		LogWarning("Invalid packet received.");
		return;
	}

	for (auto it = m_Unwrappers.begin(); it != m_Unwrappers.end(); ++it)
	{
		if (it->TryAddPacket(packet) && it->IsComplete())
		{
			auto& message = it->Unwrap<Message>();
			OnMessageReceived(message, sender);
			m_Unwrappers.erase_swap(it);
			return;
		}
	}

	PacketUnwrapper unwrapper(packet);
	if (unwrapper.IsComplete())
	{
		auto& message = unwrapper.Unwrap<Message>();
		OnMessageReceived(message, sender);
	}
	else
	{
		m_Unwrappers.emplace_back(std::move(unwrapper));
	}
}

void ServerApp::OnMessageReceived(const Message& message, const Client& sender)
{
	using namespace Console;
	using enum MessageType;
	switch (message.type)
	{
	case Connect:
	{
		uint16_t signature = Misc::GenerateUUID();
		while (m_Clients.FindBySignature(signature) || signature == 0)
		{
			// Generata again until no collision, 0 is reserved
			signature = Misc::GenerateUUID();
		}

		Message_ConnectResponse response(signature);
		auto wrapper = PacketWrapper::Wrap(response);
		wrapper.Sign(sender.signature);

		if (wrapper.Send(m_Socket, sender.address))
		{
			LogInfo(std::format("New client: {}", signature));
		}
		else
		{
			LogWsaError("Sending connect response failed");
		}
	}
	break;
	case RoomGroupRequest:
	{
		auto& request = message.As<Message_RoomGroupRequest>();
		Message_RoomGroupResponse response(request.index,
			m_PausedRooms.size() / sizeof(Message_RoomGroupResponse::GroupSize));

		size_t roomIdx = response.grpIdx * response.GroupSize;
		for (auto& entry : response.group)
		{
			if (roomIdx >= m_PausedRooms.size())
				break;

			auto& room = m_PausedRooms[roomIdx];
			if (!room.IsFull())
			{
				entry = room.uuid;
			}
			++roomIdx;
		}

		auto wrapper = PacketWrapper::Wrap(response);
		wrapper.Sign(sender.signature);
		if (!wrapper.Send(m_Socket, sender.address))
		{
			LogWsaError("Sending room group response failed");
		}
	}
	break;
	case RoomJoinRequest:
	{
		auto& request = message.As<Message_RoomJoinRequest>();

		Message_RoomJoinResponse::JoinStatus status = Message_RoomJoinResponse::Rejected;
		for (auto& room : m_PausedRooms)
		{
			if (!room.IsFull() && room.uuid == request.uuid)
			{
				if (room.leftSignature == 0)
				{
					room.leftSignature = sender.signature;
				}
				else
				{
					room.rightSignature = sender.signature;
				}

				status = Message_RoomJoinResponse::Accepted;
				break;
			}
		}

		Message_RoomJoinResponse response(status);
		auto wrapper = PacketWrapper::Wrap(response);
		wrapper.Sign(sender.signature);
		if (!wrapper.Send(m_Socket, sender.address))
		{
			LogWsaError("Sending room join response failed");
		}
	}
	break;
	case InputUpdate:
	{
		auto& update = message.As<Message_InputUpdate>();
		PaddlesBehaviour updateBehaviour = update.behaviour;

		for (auto& room : m_PlayingRooms)
		{
			if (room.leftSignature == sender.signature)
			{
				room.game.Behaviours &= ~PaddlesBehaviour::Left; // Erase left bits
				updateBehaviour &= PaddlesBehaviour::Left; // only keep left bits
				room.game.Behaviours |= updateBehaviour; // merge both
			}
			else if (room.rightSignature == sender.signature)
			{
				room.game.Behaviours &= ~PaddlesBehaviour::Right; // Erase right bits
				updateBehaviour &= PaddlesBehaviour::Right; // only keep right bits
				room.game.Behaviours |= updateBehaviour; // merge both
			}
		}
	}
	break;
	}
}

void ServerApp::FlushLostPackets(TimePoint now)
{
	size_t count = 0;
	for (auto it = m_Unwrappers.begin(); it != m_Unwrappers.end();)
	{
		if (now - it->Timestamp() > TimeForLostPacket)
		{
			m_Unwrappers.erase_swap(it);
			++count;
		}
		++it;
	}

	if (count > 0)
		LogWarning(std::format("Flushed lost packets: {} partial message(s) removed", count));
}

void ServerApp::CleanupDirectory(TimePoint now)
{
	if (m_LastClientsCleanup + ClientDirectoryCleanupInterval > now)
	{
		size_t count = m_Clients.RemoveIfLastContactBefore(now - ClientDirectoryMaxLastContact);
		m_LastClientsCleanup = now;
		LogInfo(std::format("Directory cleaned: {} client(s) removed", count));
	}
}

void ServerApp::MaintainRooms(TimePoint now, float dt)
{
	for (auto it = m_PausedRooms.begin(); it != m_PausedRooms.end();)
	{
		PongRoom& room = *it;
		bool leftActive = now - room.leftLastUpdate < InactivityUpdateMissingTime;
		bool rightActive = now - room.rightLastUpdate < InactivityUpdateMissingTime;

		if (leftActive && rightActive)
		{
			// Missing client reconnected / joined
			m_PlayingRooms.emplace_back(std::move(room));
		}
		else if (leftActive || rightActive)
		{
			// Still one client connected
			auto* leftClient = m_Clients.FindBySignature(room.leftSignature);
			auto* rightClient = m_Clients.FindBySignature(room.rightSignature);

			Message_GameUpdate update(room.game, Message_GameUpdate::Paused, room.leftScore, room.rightScore);
			auto wrapper = PacketWrapper::Wrap(update);

			wrapper.Sign(room.leftSignature);
			wrapper.Send(m_Socket, leftClient->address);

			wrapper.Sign(room.rightSignature);
			wrapper.Send(m_Socket, rightClient->address);

			++it;
			continue;
		}

		// Both clients are connected or disconnected
		m_PausedRooms.erase_swap(it);
	}

	for (auto it = m_PausedRooms.begin(); it != m_PausedRooms.end();)
	{
		PongRoom& room = *it;
		bool leftActive = now - room.leftLastUpdate < InactivityUpdateMissingTime;
		bool rightActive = now - room.rightLastUpdate < InactivityUpdateMissingTime;

		if (!(leftActive && rightActive))
		{
			// Client disconnected
			m_PausedRooms.emplace_back(std::move(room));
			continue;
		}

		room.game.Update(dt);
		switch (room.game.GameStateInfos())
		{
		case GameStateInfo::LeftWins:
		{
			++room.leftScore;
			room.game.Reset();
			break;
		}
		case GameStateInfo::RightWins:
		{
			++room.rightScore;
			room.game.Reset();
			break;
		}
		}

		auto* leftClient = m_Clients.FindBySignature(room.leftSignature);
		auto* rightClient = m_Clients.FindBySignature(room.rightSignature);

		Message_GameUpdate update(room.game, Message_GameUpdate::Playing, room.leftScore, room.rightScore);
		auto wrapper = PacketWrapper::Wrap(update);

		wrapper.Sign(room.leftSignature);
		wrapper.Send(m_Socket, leftClient->address);

		wrapper.Sign(room.rightSignature);
		wrapper.Send(m_Socket, rightClient->address);

		++it;
	}
}

void ServerApp::LogInfo(std::string_view info) const
{
	using namespace Console;
	Out << TextColors::BrightFgBlack << '['
		<< TextColors::FgCyan << "Info"
		<< TextColors::BrightFgBlack << "] "
		<< TextColors::Reset << info << '\n';
}

void ServerApp::LogWarning(std::string_view warning) const
{
	using namespace Console;
	Out << TextColors::BrightFgBlack << '['
		<< TextColors::FgYellow << "Warning"
		<< TextColors::BrightFgBlack << "] "
		<< TextColors::Reset << warning << '\n';
}

void ServerApp::LogError(std::string_view error) const
{
	using namespace Console;
	Out << TextColors::BrightFgBlack << '['
		<< TextColors::FgRed << "Error"
		<< TextColors::BrightFgBlack << "] "
		<< TextColors::Reset << error << '\n';
}

void ServerApp::LogWsaError(std::string_view what, int error) const
{
	using namespace Console;
	Out << TextColors::BrightFgBlack << '['
		<< TextColors::FgRed << "WsaError"
		<< TextColors::BrightFgBlack << "] "
		<< TextColors::Reset << what << '\n'
		<< TextColors::BrightFgBlack << "Explanation: "
		<< NetHelper::GetWsaErrorExplanation(error)
		<< '\n';
}
