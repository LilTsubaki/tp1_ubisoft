//**********************************************************************************************************************
#include "Log.h"
#include "Time.h"
#include "Win32Console.h"

//**********************************************************************************************************************
static const char* _static_log_types_strings[] = { "Trace", "Warning", "Error", "Assert" };
static const char* _static_log_modules_strings[] = { "Game", "Client", "Server", "Session", "DataContainer" };

//**********************************************************************************************************************
void Log(LogType type, LogModule module, bool bWriteConsole, const char* format, ...)
{
	WORD wColor = SILVER;

	char str_tmp[2048];
	char* ptr = str_tmp;
	*str_tmp = 0;

	ptr += sprintf_s(str_tmp, sizeof(str_tmp), "[%s] ", _static_log_modules_strings[module]);

	if (type == LogType::eWarning)
	{
		ptr = strcpy(ptr, "Warning: ");
		wColor = YELLOW;
	}
	else if (type == LogType::eError)
	{
		ptr = strcpy(ptr, "ERROR: ");
		wColor = RED;
	}
	else if (type == LogType::eAssert)
	{
		ptr = strcpy(ptr, "ASSERT: ");
		wColor = PURPLE;
	}

	va_list argptr;
	va_start(argptr, format);
	vsprintf_s(ptr, sizeof(str_tmp) - strlen(str_tmp), format, argptr);
	va_end(argptr);

	if (bWriteConsole)
	{
		Win32Console::PrintColor(wColor, str_tmp);
	}

	char str_pid[4096];
	sprintf_s(str_pid, sizeof(str_pid), "[%llu][PID=%lu] ", uu::Time::GetSynchTime(), GetCurrentThreadId());
	strcat(str_pid, str_tmp);

	OutputDebugStringA(str_pid);
}

//**********************************************************************************************************************
void Log(WORD wColor, const char* format, ...)
{
	char str_tmp[2048];

	va_list argptr;
	va_start(argptr, format);
	vsprintf_s(str_tmp, sizeof(str_tmp), format, argptr);
	va_end(argptr);

	Win32Console::PrintColor(wColor, str_tmp);

	char str_pid[4096];
	sprintf_s(str_pid, sizeof(str_pid), "[%llu][PID=%lu] ", uu::Time::GetSynchTime(), GetCurrentThreadId());
	strcat(str_pid, str_tmp);

	OutputDebugStringA(str_pid);
}
