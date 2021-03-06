#include <SDL.h>
#include <iostream>

//#include <vld.h>

#include "Game.h"
#include "FrameTimer.h"

#include "..\Renderer\RenderQueue.h"



int main(int argc, char* args[])
{
	// Game Setup
	std::string windowName = "Window Name!";
	int screenWidth = 800;
	int screenHeight = 600;
	Game game(windowName, screenWidth, screenHeight);

	// Frame Timer Setup
	constexpr int updatesPerSecond = 60;
	constexpr float frameTime = 1.0f / updatesPerSecond;
	FrameTimer timer(updatesPerSecond);

	while (game.IsRunning())
	{
		Timestamp UpdateBegin = timer.Now();

		game.ProcessInput();
		game.Update(frameTime);
		timer.UpdateEstimatedUpdateTime(UpdateBegin);

		// lock framerate
		timer.Sleep(UpdateBegin);

		Timestamp renderBegin = timer.Now();
		game.Render();
		timer.UpdateEstimatedRenderTime(renderBegin);

		timer.PrintDebugStats(); // Frame timing
	}

	return EXIT_SUCCESS;
}