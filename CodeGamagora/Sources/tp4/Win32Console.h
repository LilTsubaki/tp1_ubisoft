//**********************************************************************************************************************
#pragma once

#include <Windows.h>

//**********************************************************************************************************************
#define		BLACK		0x00
#define		GRAY		(BLACK|FOREGROUND_INTENSITY)
#define		NAVY		0x01
#define		BLUE		(NAVY|FOREGROUND_INTENSITY)
#define		GREEN		0x02
#define		LIME		(GREEN|FOREGROUND_INTENSITY)
#define		TEAL		0x03
#define		AQUA		(TEAL|FOREGROUND_INTENSITY)
#define		CYAN		AQUA
#define		MARRON		0x04
#define		RED			(MARRON|FOREGROUND_INTENSITY)
#define		PURPLE		0x05
#define		FUCHIA		(PURPLE|FOREGROUND_INTENSITY)
#define		MAGENTA		FUCHIA
#define		OLIVE		0x06
#define		YELLOW		(OLIVE|FOREGROUND_INTENSITY)
#define		SILVER		0x07
#define		WHITE		(SILVER|FOREGROUND_INTENSITY)

//**********************************************************************************************************************
class Win32Console
{
public:
	static void Init();
	static void Terminate();

	//Writes a character string beginning at a specified location.
	static void PrintXY(SHORT x, SHORT y, const char* format, ...);

	//Writes a character string beginning at a specified location with a specified color.
	static void PrintXYColor(SHORT x, SHORT y, WORD wColor, const char* format, ...);

	//Writes a character string beginning at the current cursor location with a specified color.
	static void PrintColor(WORD wColor, const char* format, ...);

	//Writes a character string beginning at the current cursor location.
	static void WriteString(const char* string, DWORD dwLen);

	//Retrieves information about the specified console screen buffer.
	static void GetScreenBufferInfo(CONSOLE_SCREEN_BUFFER_INFO& screenBufferInfo);

	//Gets the cursor position in the console screen.
	static void GetCursorPosition(COORD& dwCursorPosition);

	//Sets the cursor position in the console screen.
	static void SetCursorPosition(COORD const& dwCursorPosition);

	//Sets the title for the current console window.
	static void SetTitle(const char* format, ...);

	static void Minimize();
	static void Maximize();

public:
	// console output specifics
	static HANDLE hConsoleStdOut;
	static WORD wConsoleAttributes;
	static HWND hWndWindow;

};