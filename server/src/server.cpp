#include "Utils/Console.h"
#include <Windows.h>

int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, PSTR lpCmdLine, int nCmdShow)
{
	using namespace Console;
	Out << WindowTitle::SetTitle("Pong Server Console")
		<< TextColors::BrightFgYellow << ">>> The server is starting!\n";

	// TODO: server

	Out << TextColors::FgCyan << ">>> The server has been shut down.\n";

	return 0;
}
