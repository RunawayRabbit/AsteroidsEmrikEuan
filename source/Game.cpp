
#include <SDL.h>
#include <iostream> // for error messages

// Move out into Renderer
#include <SDL_image.h> 
#include <cmath> // for fmod

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
	
	SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
	
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

float AngleDelta(float a, float b)
{
	const float TAU{ 6.28318530717958647692 };
	float delta = fmod((a - b), TAU);
	float mapped = fmod(delta* 2.0f, TAU) - delta;
	return mapped;
}

void Game::Render()
{
	// All of this goes out to the renderer.
	SDL_RenderClear(renderer);

	static float t = 0;
	t += 2.0f;

	SDL_Rect asteroidSrc;
	asteroidSrc.w = 58;
	asteroidSrc.h = 61;
	asteroidSrc.x = 66;
	asteroidSrc.y = 194;

	SDL_Rect asteroidDest;
	asteroidDest.w = 58;
	asteroidDest.h = 61;
	asteroidDest.x = 500;
	asteroidDest.y = 300;
	SDL_RenderCopyEx(renderer, spritesheet, &asteroidSrc, &asteroidDest,
		t, NULL, SDL_FLIP_NONE);


	SDL_Rect shipSrc;
	shipSrc.w = 22;
	shipSrc.h = 30;
	shipSrc.x = 5;
	shipSrc.y = 1;

	SDL_Rect shipDest;
	shipDest.w = 22;
	shipDest.h = 30;
	shipDest.x = 400;
	shipDest.y = 300;

	SDL_RenderCopyEx(renderer, spritesheet, &shipSrc, &shipDest, -t/2,
		NULL, SDL_FLIP_NONE);

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