#include "ServerApp.h"
#include "Network/NetHelper.h"
#include "Network/PacketWrapper.h"
#include "Utils/Console.h"
#include "Utils/Misc.h"
#include <chrono>
#include <thread>

static auto LogPrefix(auto category)
{
	using namespace Console;
	return Out
		<< TextColors::BrightFgBlack << '['
		<< TextColors::FgWhite << category
		<< TextColors::BrightFgBlack << "] ";
}

static auto ErrPrefix() { return LogPrefix("ERR") << Console::TextColors::FgRed; }
static auto InfoPrefix() { return LogPrefix("INFO") << Console::TextColors::BrightFgBlue; }

ServerApp::ServerApp()
	: m_UpTime()
	, m_Status(Running)
	, m_WsaData()
	, m_Socket()
	, m_Addr(IpAddress::Any)
{
	int error;

	using namespace Console;
	Out << WindowTitle::SetTitle("Pong Server Console");

	if (m_WsaData.error)
	{
		ErrPrefix()
			<< "WSA initialisation failed.\nReason: "
			<< NetHelper::GetWsaErrorExplanation(m_WsaData.error)
			<< "\n";

		goto init_failed;
	}

	LogPrefix("INIT")
		<< TextColors::FgCyan
		<< "WSA initialisation OK.\n";

	if (!m_Socket.IsValid())
	{
		ErrPrefix()
			<< "Socket creation failed.\nReason: "
			<< NetHelper::GetWsaErrorExplanation()
			<< "\n";

		goto init_failed;
	}
	LogPrefix("INIT")
		<< TextColors::FgCyan
		<< "Socket creation OK.\n";

	error = m_Socket.BindTo(m_Addr);
	if (error)
	{

		ErrPrefix()
			<< "Socket bind failed.\nReason: "
			<< NetHelper::GetWsaErrorExplanation(error)
			<< "\n";

		goto init_failed;
	}
	LogPrefix("INIT")
		<< TextColors::FgCyan
		<< "Socket bind OK.\n";

	LogPrefix("INIT")
		<< TextColors::FgCyan
		<< "Initialization successful.\n";

	return;

init_failed:

	m_Status = InitFailed;
	ErrPrefix()
		<< TextColors::FgRed
		<< "Initialization failed!\n";
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

	do
	{
		CheckQuitKeyPressed();
		CheckPendingPackets();

		// Sleep for 1ms to avoid 100% CPU usage
		using namespace std::chrono_literals;
		std::this_thread::sleep_for(1ms);
	} while (m_Status == Running);

	return EXIT_SUCCESS;
}

ServerApp::~ServerApp()
{
	using namespace Console;
	LogPrefix("QUIT")
		<< TextColors::FgCyan
		<< "The server is shuting down.\n"
		<< TextColors::BrightFgBlack
		<< "Reason: ";

	switch (m_Status)
	{
	case InitFailed:
		Out << "Initialization failed.\n";
		break;
	case ErrorWhileRunning:
		Out << "Error while running.\n";
		break;
	case QuitRequest:
		Out << "Quit requested.\n";
		break;
	default:
		Out << "Unknown.\n";
		break;
	}

	LogPrefix("QUIT")
		<< TextColors::FgGreen
		<< "Shutdown successful.\n";
}

#include <conio.h>
void ServerApp::CheckQuitKeyPressed()
{
	constexpr auto EscapeKey = '\033';
	while (_kbhit())
	{
		if (_getch() == EscapeKey)
		{
			InfoPrefix() << "Server quit key pressed.\n";
			m_Status = QuitRequest;
		}
	}
}

void ServerApp::CheckPendingPackets()
{
	while (m_Socket.CheckPendingPacket(1))
	{
		Packet packet;
		IpAddress sender;
		if (m_Socket.ReceivePacket(packet, sender))
		{
			m_Clients.AddOrUpdate({packet.header.signature, sender});
			OnPacketReceived(packet);
		}
		else
		{
			ErrPrefix()
				<< "Error while receiving packet: "
				<< NetHelper::GetWsaErrorExplanation()
				<< "\n";
		}
	}
}

void ServerApp::OnPacketReceived(const Packet& packet)
{
	if (!packet.IsValid())
	{
		ErrPrefix() << "Invalid packet received.\n";
		return;
	}

	for (auto it = m_Unwrappers.begin(); it != m_Unwrappers.end(); ++it)
	{
		if (it->TryAddPacket(packet) && it->IsComplete())
		{
			auto& message = it->Unwrap<Message>();
			OnMessageReceived(message, it->Signature());

			// Remove it from the list
			if (std::next(it) != m_Unwrappers.end())
			{
				*it = std::move(m_Unwrappers.back());
			}
			m_Unwrappers.pop_back();
			return;
		}
	}

	PacketUnwrapper unwrapper(packet);
	if (unwrapper.IsComplete())
	{
		auto& message = unwrapper.Unwrap<Message>();
		OnMessageReceived(message, unwrapper.Signature());
	}
	else
	{
		m_Unwrappers.emplace_back(std::move(unwrapper));
	}
}

void ServerApp::OnMessageReceived(const Message& message, uint16_t sender)
{
	using namespace Console;
	using enum MessageType;
	switch (message.type)
	{
	case Reconnect:
	{
		// Delete previous signature and send back new one
		auto& reconnect = message.As<Message_Reconnect>();
		m_Clients.RemoveBySignature(reconnect.signature);
		OnMessageReceived(message.As<Message_Connect>(), sender);
	}
	break;
	case Connect:
	{
		uint16_t signature = Misc::GenerateUUID();
		while (m_Clients.FindBySignature(signature))
		{
			// Generata again until no collision
			signature = Misc::GenerateUUID();
		}

		Message_ConnectResponse response(signature);
		auto wrapper = PacketWrapper::Wrap(response);
		wrapper.Sign(sender);

		auto client = m_Clients.FindBySignature(sender);
		if (wrapper.Send(m_Socket, client->address))
		{
			InfoPrefix() << "New client: " << signature << "\n";
		}
		else
		{
			ErrPrefix()
				<< "Sending packet response failed:"
				<< NetHelper::GetWsaErrorExplanation()
				<< "\n";
		}
	}
	break;
	}
}
