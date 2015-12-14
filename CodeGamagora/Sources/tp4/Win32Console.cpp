//**********************************************************************************************************************
#include "Win32Console.h"

#include <stdio.h>
#include <stdarg.h>

//**********************************************************************************************************************
HANDLE Win32Console::hConsoleStdOut = NULL;
WORD Win32Console::wConsoleAttributes = SILVER;
HWND Win32Console::hWndWindow = NULL;

//**********************************************************************************************************************
/*static */void Win32Console::Init()
{
	//Get a console handle
	hWndWindow = GetConsoleWindow();

	hConsoleStdOut = GetStdHandle(STD_OUTPUT_HANDLE);

	CONSOLE_SCREEN_BUFFER_INFO screenBufferInfo;
	if (GetConsoleScreenBufferInfo(hConsoleStdOut, &screenBufferInfo))
	{
		wConsoleAttributes = screenBufferInfo.wAttributes;
	}
}

//**********************************************************************************************************************
/*static */void Win32Console::Terminate()
{
}

//**********************************************************************************************************************
/*static */void Win32Console::WriteString(const char* string, DWORD dwLen)
{
	DWORD nNumberOfCharsWritten = 0;
	WriteConsole(hConsoleStdOut, string, dwLen, &nNumberOfCharsWritten, NULL);
}

//**********************************************************************************************************************
/*static */void Win32Console::PrintXY(SHORT x, SHORT y, const char* format, ...)
{
	CONSOLE_SCREEN_BUFFER_INFO screenBufferInfo;
	GetConsoleScreenBufferInfo(hConsoleStdOut, &screenBufferInfo);

	COORD dwCursorPosition;
	dwCursorPosition.X = x;
	dwCursorPosition.Y = y;
	SetConsoleCursorPosition(hConsoleStdOut, dwCursorPosition);

	va_list argptr;
	va_start(argptr, format);
	vprintf(format, argptr);
	va_end(argptr);

	SetConsoleCursorPosition(hConsoleStdOut, screenBufferInfo.dwCursorPosition);
}

//**********************************************************************************************************************
/*static */void Win32Console::PrintXYColor(SHORT x, SHORT y, WORD wColor, const char* format, ...)
{
	CONSOLE_SCREEN_BUFFER_INFO screenBufferInfo;
	GetConsoleScreenBufferInfo(hConsoleStdOut, &screenBufferInfo);

	COORD dwCursorPosition;
	dwCursorPosition.X = x;
	dwCursorPosition.Y = y;
	SetConsoleCursorPosition(hConsoleStdOut, dwCursorPosition);

	SetConsoleTextAttribute(hConsoleStdOut, wColor);

	va_list argptr;
	va_start(argptr, format);
	vprintf(format, argptr);
	va_end(argptr);

	SetConsoleTextAttribute(hConsoleStdOut, screenBufferInfo.wAttributes);

	SetConsoleCursorPosition(hConsoleStdOut, screenBufferInfo.dwCursorPosition);
}

//**********************************************************************************************************************
/*static */void Win32Console::PrintColor(WORD wColor, const char* format, ...)
{
	// set the color
	SetConsoleTextAttribute(hConsoleStdOut, wColor);

	va_list argptr;
	va_start(argptr, format);
	vprintf(format, argptr);
	va_end(argptr);
	
	// restore the color
	SetConsoleTextAttribute(hConsoleStdOut, wConsoleAttributes);
}
//**********************************************************************************************************************
/*static */void Win32Console::GetScreenBufferInfo(CONSOLE_SCREEN_BUFFER_INFO& screenBufferInfo)
{
	GetConsoleScreenBufferInfo(hConsoleStdOut, &screenBufferInfo);
}

//**********************************************************************************************************************
/*static */void Win32Console::GetCursorPosition(COORD& dwCursorPosition)
{
	CONSOLE_SCREEN_BUFFER_INFO screenBufferInfo;
	GetConsoleScreenBufferInfo(hConsoleStdOut, &screenBufferInfo);

	dwCursorPosition = screenBufferInfo.dwCursorPosition;
}

//**********************************************************************************************************************
/*static */void Win32Console::SetCursorPosition(COORD const& dwCursorPosition)
{
	SetConsoleCursorPosition(hConsoleStdOut, dwCursorPosition);
}

//**********************************************************************************************************************
/*static */void Win32Console::Minimize()
{
	ShowWindow(hWndWindow, SW_MINIMIZE);
}

//**********************************************************************************************************************
/*static */void Win32Console::Maximize()
{
	ShowWindow(hWndWindow, SW_MAXIMIZE);
}

//**********************************************************************************************************************
/*static */void Win32Console::SetTitle(const char* format, ...)
{
	char str_title[1024];

	va_list argptr;
	va_start(argptr, format);
	vsprintf_s(str_title, sizeof(str_title), format, argptr);
	va_end(argptr);

	SetConsoleTitleA(str_title);
}
