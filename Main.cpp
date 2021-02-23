
#include <iostream>
#include <SDL.h>

#include "Game.h"
#include "FrameTimer.h"


int main(int argc, char* args[])
{
	// Game Setup
	std::string windowName = "Window Name!";
	int screenWidth = 800;
	int screenHeight = 600;
	Game game(windowName, screenWidth, screenHeight);

	// Frame Timer Setup
	const int updatesPerSecond = 60;
	FrameTimer timer(updatesPerSecond);

	while (game.IsRunning())
	{
		Timestamp frameBegin = timer.Now();

		game.ProcessInput();
		game.Update();

		// lock framerate
		timer.Sleep(frameBegin);

		Timestamp renderBegin = timer.Now();
		game.Render();
		timer.UpdateEstimatedRenderTime(renderBegin);

		timer.PrintDebugStats(); // Frame timing
	}

	return EXIT_SUCCESS;
}