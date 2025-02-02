#include "Console.h"
#include <Windows.h>

namespace Console
{

Console Out;

Console::Console()
{
	if (this != std::addressof(Out))
		return;

	if (GetConsoleWindow() == NULL)
	{
		AllocConsole();
		FILE* stream;
		freopen_s(&stream, "CONIN$", "r", stdin);
		freopen_s(&stream, "CONOUT$", "w", stdout);
		freopen_s(&stream, "CONOUT$", "w", stderr);
	}

	HANDLE hOut = GetStdHandle(STD_OUTPUT_HANDLE);
	DWORD dwMode = 0;
	GetConsoleMode(hOut, &dwMode);
	dwMode |= ENABLE_VIRTUAL_TERMINAL_PROCESSING;
	SetConsoleMode(hOut, dwMode);

	Out << CursorVisibility::HideCursor; // Hide cursor
}

Console::~Console()
{
	if (this != std::addressof(Out))
		return;

	Out << TextColors::Reset
		<< "\n\nPress enter to close the console...";

	std::cin.get();
	FreeConsole();
}

}
