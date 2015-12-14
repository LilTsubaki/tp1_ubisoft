//**********************************************************************************************************************
#pragma once

#include "types.h"

//**********************************************************************************************************************
enum LogType
{
	eTrace = 0,
	eWarning,
	eError,
	eAssert,
};

//**********************************************************************************************************************
enum LogModule
{
	eGame = 0,
	eClient,
	eServer,
	eSession,
	eDataContainer,
};

//**********************************************************************************************************************
extern void Log(LogType type, LogModule module, bool bWriteConsole, const char* format, ...);

//**********************************************************************************************************************
//#define LogError(_format, ...)    Log(RED, "ERROR: " _format, __VA_ARGS__);
//#define LogWarning(_format, ...)  Log(YELLOW, "Warning: " _format, __VA_ARGS__);

//#define Log(LogType::eTrace, LogModule::eDataContainer, true, _format, ...) Log(PURPLE, _format, __VA_ARGS__);
//#define LogGame(_format, ...) Log(CYAN, _format, __VA_ARGS__);
//#define LogClient(_format, ...) Log(CYAN, _format, __VA_ARGS__);
//#define LogSession(_format, ...) Log(CYAN, _format, __VA_ARGS__);

//**********************************************************************************************************************
