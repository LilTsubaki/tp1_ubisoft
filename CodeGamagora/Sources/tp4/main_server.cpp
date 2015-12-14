//**********************************************************************************************************************
#include <stdio.h>

#include "endianness.h"
#include "network.h"
#include "Win32Console.h"

#include "ServerService.h"

//**********************************************************************************************************************
int main(int argc, char **argv)
{
	Win32Console::Init();

	uu::network::Init();

	if (uu::program::Is64Bits())
		printf("64 bits program\n\n");

	if (uu::program::Is32Bits())
		printf("32 bits program\n\n");

	if (uu::system::IsBigEndian())
		printf("big-endian platform\n\n");

	if (uu::system::IsLittleEndian())
		printf("little-endian platform\n\n");

	Configuration config;
	config._listenPort = config._server_port;
	config._bBlocking = true;

	ServerService service;

	if (service.Initialize() == true)
	{
		service.FirstUpdate();

		while (service.Update() == true)
		{
		}
	}

	service.Terminate();

	uu::network::Terminate();

	Win32Console::Terminate();

	return 0;
}
