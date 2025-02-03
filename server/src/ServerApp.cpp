#include "ServerApp.h"
#include "Utils/Console.h"
#include <chrono>
#include <thread>

ServerApp::ServerApp()
{
	using namespace Console;
	Out << WindowTitle::SetTitle("Pong Server Console")
		<< TextColors::BrightFgYellow
		<< "================== Server initialization ==================\n";

	Out << TextColors::FgGreen
		<< "Initialization successful\n";

	Out << TextColors::BrightFgYellow
		<< "===========================================================\n\n";
}

int ServerApp::Run()
{
	using namespace Console;
	Out << TextColors::BrightFgYellow
		<< "===================== Server running ======================\n";

	while (!IsQuitKeyPressed())
	{
		Out << '.';

		// Sleep for 1ms to avoid 100% CPU usage
		using namespace std::chrono_literals;
		std::this_thread::sleep_for(1ms);
	}


	Out << TextColors::FgCyan
		<< "\nServer quit key pressed.\n"
		<< TextColors::BrightFgYellow
		<< "===========================================================\n\n";

	return EXIT_SUCCESS;
}

ServerApp::~ServerApp()
{
	using namespace Console;
	Out << TextColors::BrightFgYellow
		<< "===================== Server shutdown =====================\n";

	Out << TextColors::FgGreen
		<< "Shutdown successful\n";

	Out << TextColors::BrightFgYellow
		<< "===========================================================\n\n";
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
