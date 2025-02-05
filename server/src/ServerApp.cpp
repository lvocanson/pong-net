#include "ServerApp.h"
#include "Utils/Console.h"
#include <chrono>
#include <thread>
#include <Network/NetHelper.h>

static auto LogPrefix(auto category)
{
	using namespace Console;
	return Out
		<< TextColors::BrightFgBlack << '['
		<< TextColors::FgWhite << category
		<< TextColors::BrightFgBlack << "] ";
}

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
		LogPrefix("ERR")
			<< TextColors::FgRed
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
		LogPrefix("ERR")
			<< TextColors::FgRed
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

		LogPrefix("ERR")
			<< TextColors::FgRed
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
	LogPrefix("ERR")
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
			using namespace Console;
			LogPrefix("RUN")
				<< TextColors::FgCyan
				<< "Server quit key pressed.\n";

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
			OnPacketReceived(packet);
		}
		else
		{


			using namespace Console;
			LogPrefix("ERR")
				<< TextColors::FgRed
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
		using namespace Console;
		LogPrefix("ERR")
			<< TextColors::FgRed
			<< "Invalid packet received.\n";
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
	using enum MessageType;
	switch (message.type)
	{
	case Ping:
	{
		std::cout << "Ping!\n";
		break;
	}
	default:
		break;
	}
}
