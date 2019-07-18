#include "App.h"

int main()
{
	App game("Breakout", 1280, 720, 32);
	if (!game.Init())
	{
		printf("Game could not be started!");
		return 1;
	}
	else
	{
		game.Run();
	}
	return 0;
}