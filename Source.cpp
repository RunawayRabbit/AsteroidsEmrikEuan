
#include <iostream>
#include <SDL.h>

void ExitWithSDLError(const std::string errorMessage)
{
	std::cout << errorMessage << ": " << SDL_GetError() << std::endl;
	system("pause");
}

int main(int argc, char* args[])
{
	int screenWidth = 800;
	int screenHeight = 600;

	if (SDL_Init(SDL_INIT_EVERYTHING) < 0)
		ExitWithSDLError("Error initializing SDL");

	auto window = SDL_CreateWindow("winD'OH",
		SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED,
		screenWidth, screenHeight, SDL_WINDOW_SHOWN);
	if (!window)
		ExitWithSDLError("Error creating window");


	auto renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);
	if (!renderer)
		ExitWithSDLError("Error getting renderer from window");


	auto surface = SDL_GetWindowSurface(window);
	if (!surface)
		ExitWithSDLError("Error getting surface");

	auto texture = SDL_CreateTextureFromSurface(renderer, surface);
	if (!texture)
		ExitWithSDLError("Error converting Surface to Texture");

	std::cout << "Yea it's all fine.\n";

	SDL_Delay(5000);
	SDL_Quit();

	return EXIT_SUCCESS;
}