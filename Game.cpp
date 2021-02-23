
#include <SDL.h>
#include <iostream> // for error messages

#include "Game.h"

Game::Game(std::string windowName, int width, int height)
{
	if (SDL_Init(SDL_INIT_EVERYTHING) < 0)
		ExitWithSDLError("Error initializing SDL");

	auto window = SDL_CreateWindow("winD'OH",
		SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED,
		width, height, SDL_WINDOW_SHOWN);
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
}

bool Game::IsRunning() const
{
	return isRunning;
}

void Game::ProcessInput()
{
	//stub
}

void Game::Update()
{
	//stub
}

void Game::Render()
{
	//stub
	SDL_Delay(4);
}

void Game::Quit()
{
	isRunning = false;
}

void Game::ExitWithSDLError(const std::string errorMessage) const
{
	std::cout << errorMessage << ": " << SDL_GetError() << std::endl;
	system("pause");
}