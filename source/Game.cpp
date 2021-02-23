
#include <SDL.h>
#include <iostream> // for error messages

#include <SDL_image.h> // Move out into Renderer

#include "Game.h"

Game::Game(std::string windowName, int width, int height)
{
	isRunning = true;

	if (SDL_Init(SDL_INIT_EVERYTHING) < 0)
		ExitWithSDLError("Error initializing SDL");

	window = SDL_CreateWindow("winD'OH",
		SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED,
		width, height, SDL_WINDOW_SHOWN);
	if (!window)
		ExitWithSDLError("Error creating window");

	renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);
	if (!renderer)
		ExitWithSDLError("Error getting renderer from window");
	
	SDL_SetRenderDrawColor(renderer, 255, 0, 0, 255);
	
	if (!(IMG_Init(IMG_INIT_PNG) & IMG_INIT_PNG))
	{
		ExitWithSDLError("Error initializing PNG extensions");
	}

	spritesheet = LoadSpritemap("resources/asteroids-arcade.png");
}

SDL_Texture* Game::LoadSpritemap(const std::string path) const
{
	SDL_Surface* surf = IMG_Load(path.c_str());

	if (!surf)
	{
		ExitWithSDLError("Failed to load " + path + ".\n");
	}

	SDL_Texture* tex = SDL_CreateTextureFromSurface(renderer, surf);
	if (!tex)
	{
		SDL_FreeSurface(surf);
		SDL_DestroyTexture(tex);
		ExitWithSDLError("Failed to convert " + path + " to a texture.\n");
	}

	SDL_FreeSurface(surf);


	return tex;
}

Game::~Game()
{
	// Move this out into Renderer
	SDL_DestroyRenderer(renderer);
	SDL_DestroyWindow(window);
	SDL_DestroyTexture(spritesheet);


	SDL_Quit();
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
	SDL_RenderClear(renderer);

	SDL_RenderPresent(renderer);
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