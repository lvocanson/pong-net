#include "ServerApp.h"
#include "Utils/Console.h"
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

ServerApp::ServerApp()
	: m_UpTime()
	, m_Status(Running)
	, m_WsaData()
	, m_Socket()
	, m_Addr(NetHelper::UdpAddress::Any)
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
		if (IsQuitKeyPressed())
		{
			LogPrefix("RUN")
				<< TextColors::FgCyan
				<< "Server quit key pressed.\n";

			m_Status = QuitRequest;
		}

		// TODO: encapsulate
		char buffer[512];
		int size = NetHelper::UdpAddress::size();
		int bytesReceived = recvfrom(m_Socket, buffer, sizeof(buffer) - 1, 0, &m_Addr, &size);
		if (bytesReceived == SOCKET_ERROR)
		{
			int error = WSAGetLastError();
			if (error != WSAEWOULDBLOCK)
			{
				LogPrefix("ERR")
					<< TextColors::FgRed
					<< "Error while receiving: "
					<< NetHelper::GetWsaErrorExplanation(error)
					<< "\n";
			}
		}
		else
		{
			buffer[bytesReceived] = '\0';

			LogPrefix("RUN")
				<< TextColors::FgCyan
				<< "Message received:\n"
				<< TextColors::BrightFgBlack
				<< buffer
				<< "\n";
		}

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
bool ServerApp::IsQuitKeyPressed()
{
	constexpr auto EscapeKey = '\033';

	while (_kbhit())
		if (_getch() == EscapeKey)
			return true;

	return false;
}
