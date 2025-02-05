#include "ServerApp.h"
#include "Network/NetHelper.h"
#include "Network/PacketWrapper.h"
#include "Utils/Console.h"
#include "Utils/Misc.h"
#include <chrono>
#include <thread>


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

	do
	{
		UpdateStatus();
		HandlePendingPackets();

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
