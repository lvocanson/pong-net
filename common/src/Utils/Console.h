#pragma once
#include <iostream>
#include <string>

namespace Console
{

/// Full documentation at https://learn.microsoft.com/en-us/windows/console/console-virtual-terminal-sequences

namespace TextColors
{
inline constexpr auto Reset = "\x1b[0m";
inline constexpr auto Bold = "\x1b[1m";
inline constexpr auto NoBold = "\x1b[22m";
inline constexpr auto Underline = "\x1b[4m";
inline constexpr auto NoUnderline = "\x1b[24m";
inline constexpr auto Negative = "\x1b[7m";
inline constexpr auto Positive = "\x1b[27m";

inline constexpr auto FgBlack = "\x1b[30m";
inline constexpr auto FgRed = "\x1b[31m";
inline constexpr auto FgGreen = "\x1b[32m";
inline constexpr auto FgYellow = "\x1b[33m";
inline constexpr auto FgBlue = "\x1b[34m";
inline constexpr auto FgMagenta = "\x1b[35m";
inline constexpr auto FgCyan = "\x1b[36m";
inline constexpr auto FgWhite = "\x1b[37m";
inline constexpr auto FgExtended = "\x1b[38m";
inline constexpr auto FgDefault = "\x1b[39m";

inline constexpr auto BgBlack = "\x1b[40m";
inline constexpr auto BgRed = "\x1b[41m";
inline constexpr auto BgGreen = "\x1b[42m";
inline constexpr auto BgYellow = "\x1b[43m";
inline constexpr auto BgBlue = "\x1b[44m";
inline constexpr auto BgMagenta = "\x1b[45m";
inline constexpr auto BgCyan = "\x1b[46m";
inline constexpr auto BgWhite = "\x1b[47m";
inline constexpr auto BgExtended = "\x1b[48m";
inline constexpr auto BgDefault = "\x1b[49m";

inline constexpr auto BrightFgBlack = "\x1b[90m";
inline constexpr auto BrightFgRed = "\x1b[91m";
inline constexpr auto BrightFgGreen = "\x1b[92m";
inline constexpr auto BrightFgYellow = "\x1b[93m";
inline constexpr auto BrightFgBlue = "\x1b[94m";
inline constexpr auto BrightFgMagenta = "\x1b[95m";
inline constexpr auto BrightFgCyan = "\x1b[96m";
inline constexpr auto BrightFgWhite = "\x1b[97m";

inline constexpr auto BrightBgBlack = "\x1b[100m";
inline constexpr auto BrightBgRed = "\x1b[101m";
inline constexpr auto BrightBgGreen = "\x1b[102m";
inline constexpr auto BrightBgYellow = "\x1b[103m";
inline constexpr auto BrightBgBlue = "\x1b[104m";
inline constexpr auto BrightBgMagenta = "\x1b[105m";
inline constexpr auto BrightBgCyan = "\x1b[106m";
inline constexpr auto BrightBgWhite = "\x1b[107m";
}

namespace CursorPositioning
{
inline std::string CursorUp(int n = 1)
{
	return "\x1b[" + std::to_string(n) + "A";
}
inline std::string CursorDown(int n = 1)
{
	return "\x1b[" + std::to_string(n) + "B";
}
inline std::string CursorForward(int n = 1)
{
	return "\x1b[" + std::to_string(n) + "C";
}
inline std::string CursorBackward(int n = 1)
{
	return "\x1b[" + std::to_string(n) + "D";
}
inline std::string CursorNextLine(int n = 1)
{
	return "\x1b[" + std::to_string(n) + "E";
}
inline std::string CursorPreviousLine(int n = 1)
{
	return "\x1b[" + std::to_string(n) + "F";
}
inline std::string CursorHorizontalAbsolute(int n = 1)
{
	return "\x1b[" + std::to_string(n) + "G";
}
inline std::string VerticalPositionAbsolute(int n = 1)
{
	return "\x1b[" + std::to_string(n) + "d";
}
inline std::string CursorPosition(int row = 1, int col = 1)
{
	return "\x1b[" + std::to_string(row) + ";" + std::to_string(col) + "H";
}
inline std::string HorizontalVerticalPosition(int row = 1, int col = 1)
{
	return "\x1b[" + std::to_string(row) + ";" + std::to_string(col) + "f";
}
}

namespace CursorVisibility
{
inline constexpr auto EnableBlink = "\x1b[?12h";
inline constexpr auto DisableBlink = "\x1b[?12l";
inline constexpr auto ShowCursor = "\x1b[?25h";
inline constexpr auto HideCursor = "\x1b[?25l";
}

namespace CursorShape
{
inline constexpr auto Default = "\x1b[0 q";
inline constexpr auto BlinkingBlock = "\x1b[1 q";
inline constexpr auto SteadyBlock = "\x1b[2 q";
inline constexpr auto BlinkingUnderline = "\x1b[3 q";
inline constexpr auto SteadyUnderline = "\x1b[4 q";
inline constexpr auto BlinkingBar = "\x1b[5 q";
inline constexpr auto SteadyBar = "\x1b[6 q";
}

namespace Viewport
{
inline std::string ScrollUp(int n = 1)
{
	return "\x1b[" + std::to_string(n) + "S";
}
inline std::string ScrollDown(int n = 1)
{
	return "\x1b[" + std::to_string(n) + "T";
}
}

namespace TextModification
{
inline std::string InsertCharacter(int n = 1)
{
	return "\x1b[" + std::to_string(n) + "@";
}
inline std::string DeleteCharacter(int n = 1)
{
	return "\x1b[" + std::to_string(n) + "P";
}
inline std::string EraseCharacter(int n = 1)
{
	return "\x1b[" + std::to_string(n) + "X";
}
inline std::string InsertLine(int n = 1)
{
	return "\x1b[" + std::to_string(n) + "L";
}
inline std::string DeleteLine(int n = 1)
{
	return "\x1b[" + std::to_string(n) + "M";
}
}

namespace Erase
{
inline std::string EraseInDisplay(int n = 0)
{
	return "\x1b[" + std::to_string(n) + "J";
}
inline std::string EraseInLine(int n = 0)
{
	return "\x1b[" + std::to_string(n) + "K";
}
}

namespace ScreenColors
{

inline std::string SetPalette(int index, int r, int g, int b)
{
	return "\x1b]4;" + std::to_string(index) + ";rgb:" +
		std::to_string(r) + "/" + std::to_string(g) + "/" + std::to_string(b) + "\x07";
}
}

namespace ModeChanges
{
inline constexpr auto EnableKeypadAppMode = "\x1b=";
inline constexpr auto EnableKeypadNumMode = "\x1b>";
inline constexpr auto EnableCursorKeysAppMode = "\x1b[?1h";
inline constexpr auto DisableCursorKeysAppMode = "\x1b[?1l";
}

namespace QueryState
{
inline constexpr auto ReportCursorPosition = "\x1b[6n";
inline constexpr auto DeviceAttributes = "\x1b[0c";
}

namespace Tabs
{
inline constexpr auto HorizontalTabSet = "\x1bH";
inline std::string CursorHorizontalTab(int n = 1)
{
	return "\x1b[" + std::to_string(n) + "I";
}
inline std::string CursorBackwardTab(int n = 1)
{
	return "\x1b[" + std::to_string(n) + "Z";
}
inline constexpr auto TabClearCurrent = "\x1b[0g";
inline constexpr auto TabClearAll = "\x1b[3g";
}

namespace DesignateCharset
{
inline constexpr auto DECLineDrawing = "\x1b(0";
inline constexpr auto USASCII = "\x1b(B";
}

namespace ScrollingMargins
{
inline std::string SetScrollingRegion(int top, int bottom)
{
	return "\x1b[" + std::to_string(top) + ";" + std::to_string(bottom) + "r";
}
}

namespace WindowTitle
{
inline std::string SetTitle(const std::string& title)
{
	return "\x1b]0;" + title + "\x07";
}
inline std::string SetTitleAlternate(const std::string& title)
{
	return "\x1b]2;" + title + "\x07";
}
}

namespace AlternateScreen
{
inline constexpr auto Enter = "\x1b[?1049h";
inline constexpr auto Exit = "\x1b[?1049l";
}

namespace WindowWidth
{
inline constexpr auto SetColumns132 = "\x1b[?3h";
inline constexpr auto SetColumns80 = "\x1b[?3l";
}

namespace SoftReset
{
inline constexpr auto Reset = "\x1b[!p";
}

struct Console
{
	Console();
	~Console();

	Console& operator<<(const auto& value)
	{
		std::cout << value;
		return *this;
	}

} extern Out;

}
