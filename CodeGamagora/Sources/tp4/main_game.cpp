//**********************************************************************************************************************
#include "network.h"
#include "Win32Console.h"

#include "Game.h"

//**********************************************************************************************************************
int main(int argc, char **argv)
{
	srand((unsigned int)time(nullptr));

	Win32Console::Init();

	uu::network::Init();

	Configuration config;
	config._bBroadcast = true;
	config._bBlocking = false;

	Game game;
	
	if (game.Initialize() == true)
	{
		game.FirstUpdate();

		while (game.Update() == true)
		{
		}
	}

	game.Terminate();

	uu::network::Terminate();

	Win32Console::Terminate();

	return 0;
}
