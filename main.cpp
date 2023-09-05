#include "Framework.h"

int WINAPI WinMain(HINSTANCE, HINSTANCE, LPSTR, int)
{
	Framework* game = new Framework();

	game->Run();

	delete game;

	return 0;
}