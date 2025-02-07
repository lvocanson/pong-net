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
		<< "The server is running at local address "
		<< TextColors::BrightFgMagenta
		<< IpAddress::LocalAddress().ToPhrase().View()
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
				.lastPacketSent = packet.header.timestamp,
				.address = sender});
			OnPacketReceived(packet, client);
		}
		else
		{
			int error = WSAGetLastError();
			if (error == WSAECONNRESET)
				// Received if we try to send packets to closed client
				continue;

			LogWsaError("Error while receiving packet", error);
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
	case QuickMatchRequest:
	{
		// Find not full room or create one, then send the confirmation
		PongRoom* matchRoom = nullptr;
		for (auto& room : m_PausedRooms)
		{
			if (room.ContainsSignature(sender.signature))
			{
				matchRoom = &room;
				break;
			}

			if (room.leftSignature == 0)
			{
				room.leftSignature = sender.signature;
				matchRoom = &room;
				break;
			}
			if (room.rightSignature == 0)
			{
				room.rightSignature = sender.signature;
				matchRoom = &room;
				break;
			}
		}

		if (!matchRoom)
		{
			matchRoom = &CreateRoom(sender);
		}

		Message_RoomJoinResponse response(Message_RoomJoinResponse::Accepted, matchRoom->uuid);
		auto wrapper = PacketWrapper::Wrap(response);
		wrapper.Sign(sender.signature);
		if (!wrapper.Send(m_Socket, sender.address))
		{
			LogWsaError("Sending quick match response failed");
		}

		Message_GetIdRoomResponse responseId(matchRoom->uuid);
		auto wrapperId = PacketWrapper::Wrap(responseId);
		wrapperId.Sign(sender.signature);
		if (!wrapperId.Send(m_Socket, sender.address))
		{
			LogWsaError("Add room response failed");
		}
	}
	break;
	case RoomCreationRequest:
	{
		auto& room = CreateRoom(sender);

		Message_RoomJoinResponse response(Message_RoomJoinResponse::Accepted, room.uuid);
		auto wrapper = PacketWrapper::Wrap(response);
		wrapper.Sign(sender.signature);
		if (!wrapper.Send(m_Socket, sender.address))
		{
			LogWsaError("Sending room creation response failed");
		}

		Message_GetIdRoomResponse responseId(room.uuid);
		auto wrapperId = PacketWrapper::Wrap(responseId);
		wrapperId.Sign(sender.signature);
		if (!wrapperId.Send(m_Socket, sender.address))
		{
			LogWsaError("Add room response failed");
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

		Message_RoomJoinResponse response(status, request.uuid);
		auto wrapper = PacketWrapper::Wrap(response);
		wrapper.Sign(sender.signature);
		if (!wrapper.Send(m_Socket, sender.address))
		{
			LogWsaError("Sending room join response failed");
		}
	}
	break;
	case GetIdRoomRequest:
	{
		auto& request = message.As<Message_GetIdRoomRequest>();
		GetRoomIds(sender);
	}
	break;
	case GameUpdate:
	{
		auto& update = message.As<Message_GameUpdate>();

		TimePoint now = std::chrono::high_resolution_clock::now();
		float diff = std::chrono::duration<float>(now - sender.lastPacketSent).count();

		bool wasPaused = false;
		for (auto& room : m_PausedRooms)
		{
			if (room.leftSignature == sender.signature)
			{
				room.leftLastUpdate = sender.lastPacketSent;
				wasPaused = true;
			}
			else if (room.rightSignature == sender.signature)
			{
				room.rightLastUpdate = sender.lastPacketSent;
				wasPaused = true;
			}
		}

		if (wasPaused)
			// no need to update, all inputs are ignored
			break;

		Pong* pongGame = nullptr;
		for (auto& room : m_PlayingRooms)
		{
			if (room.leftSignature == sender.signature)
			{
				room.game.Behaviours &= ~PaddlesBehaviour::Left; // Erase left bits
				room.game.Behaviours |= update.game.Behaviours & PaddlesBehaviour::Left; // only keep left bits, merge both
				room.leftLastUpdate = sender.lastPacketSent;
				pongGame = &room.game;
				break;
			}
			else if (room.rightSignature == sender.signature)
			{
				room.game.Behaviours &= ~PaddlesBehaviour::Right; // Erase right bits
				room.game.Behaviours |= update.game.Behaviours & PaddlesBehaviour::Right; // only keep right bits, merge both
				room.rightLastUpdate = sender.lastPacketSent;
				pongGame = &room.game;
				break;
			}
		}

		if (!pongGame)
			// receiving input of deleted game?
			break;

		// Make up for lost time
		auto tempGame = update.game;
		tempGame.Behaviours = pongGame->Behaviours;
		tempGame.Update(diff);

		using enum PaddlesBehaviour;
		if ((pongGame->Behaviours & LeftUp) != None)
			tempGame.LeftPaddle = std::min(tempGame.LeftPaddle, pongGame->LeftPaddle);
		else if ((pongGame->Behaviours & LeftDown) != None)
			tempGame.LeftPaddle = std::max(tempGame.LeftPaddle, pongGame->LeftPaddle);
		if ((pongGame->Behaviours & RightUp) != None)
			tempGame.RightPaddle = std::min(tempGame.RightPaddle, pongGame->RightPaddle);
		else if ((pongGame->Behaviours & RightDown) != None)
			tempGame.RightPaddle = std::max(tempGame.RightPaddle, pongGame->RightPaddle);
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
			it = m_Unwrappers.erase_swap(it);
			++count;
		}
		++it;
	}

	if (count > 0)
		LogWarning(std::format("Flushed lost packets: {} partial message(s) removed", count));
}

void ServerApp::CleanupDirectory(TimePoint now)
{
	if (m_LastClientsCleanup + ClientDirectoryCleanupInterval < now)
	{
		TimePoint maxLastContact = now - ClientDirectoryMaxLastContact;
		size_t count = 0;

		for (auto it = m_Clients.m_Directory.begin(); it != m_Clients.m_Directory.end();)
		{
			if (it->lastPacketSent > maxLastContact)
			{
				++it;
				continue;
			}

			LogCleanup(std::format("Client {} has been inactive for too long and was forgotten", it->signature));
			it = m_Clients.m_Directory.erase_swap(it);
			++count;
		}

		m_LastClientsCleanup = now;
	}
}

PongRoom& ServerApp::CreateRoom(const Client& applicant)
{
	uint16_t uuid = 0;
	while (uuid == 0)
	{
		uuid = Misc::GenerateUUID();
		if (uuid == 0)
			continue;

		for (auto& room : m_PlayingRooms)
		{
			if (room.uuid == uuid)
			{
				uuid = 0;
				break;
			}
		}

		if (uuid == 0)
			continue;

		for (auto& room : m_PausedRooms)
		{
			if (room.uuid == uuid)
			{
				uuid = 0;
				break;
			}
		}
	}

	auto& room = m_PausedRooms.emplace_back(uuid);
	if (rand() % 2)
	{
		room.leftSignature = applicant.signature;
		room.leftLastUpdate = applicant.lastPacketSent;
	}
	else
	{
		room.rightSignature = applicant.signature;
		room.rightLastUpdate = applicant.lastPacketSent;

	}

	LogInfo(std::format("Room {} created", uuid));

	return room;
}

void ServerApp::MaintainRooms(TimePoint now, float dt)
{
	for (auto it = m_PausedRooms.begin(); it != m_PausedRooms.end();)
	{
		PongRoom& room = *it;
		bool leftActive = room.leftLastUpdate + InactivityUpdateMissingTime > now;
		bool rightActive = room.rightLastUpdate + InactivityUpdateMissingTime > now;

		if (leftActive && rightActive)
		{
			// Missing client reconnected / joined
			m_PlayingRooms.emplace_back(std::move(room));
			it = m_PausedRooms.erase_swap(it);
			continue;
		}

		if (leftActive || rightActive)
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

		// Both clients are disconnected
		LogInfo(std::format("Room {} was closed due to disconnections", it->uuid));
		it = m_PausedRooms.erase_swap(it);
	}

	for (auto it = m_PlayingRooms.begin(); it != m_PlayingRooms.end();)
	{
		PongRoom& room = *it;
		bool leftActive = room.leftLastUpdate + InactivityUpdateMissingTime > now;
		bool rightActive = room.rightLastUpdate + InactivityUpdateMissingTime > now;

		if (!(leftActive && rightActive))
		{
			// Client disconnected
			m_PausedRooms.emplace_back(std::move(room));
			it = m_PlayingRooms.erase_swap(it);
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

void ServerApp::LogCleanup(std::string_view message) const
{
	using namespace Console;
	Out << TextColors::BrightFgBlack << '['
		<< TextColors::FgGreen << "Cleanup"
		<< TextColors::BrightFgBlack << "] "
		<< TextColors::Reset << message << '\n';
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

void ServerApp::GetRoomIds(const Client& sender) const
{
	for (auto& room : m_PlayingRooms) 
	{
		Message_GetIdRoomResponse response(room.uuid);
		auto wrapper = PacketWrapper::Wrap(response);
		wrapper.Sign(sender.signature);
		if (!wrapper.Send(m_Socket, sender.address))
		{
			LogWsaError("Sending room join response failed");
		}
	}
	for (auto& room : m_PausedRooms)
	{
		Message_GetIdRoomResponse response(room.uuid);
		auto wrapper = PacketWrapper::Wrap(response);
		wrapper.Sign(sender.signature);
		if (!wrapper.Send(m_Socket, sender.address))
		{
			LogWsaError("Sending room join response failed");
		}
	}
}
