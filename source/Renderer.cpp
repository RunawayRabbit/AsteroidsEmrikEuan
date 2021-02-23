#pragma once

#include <SDL.h>
#include <SDL_image.h> // PNG imports
#include <iostream> // for error reporting
#include "Renderer.h"

Renderer::Renderer(std::string windowName, int width, int height)
{
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
Renderer::~Renderer()
{
	SDL_DestroyRenderer(renderer);
	SDL_DestroyWindow(window);
	SDL_DestroyTexture(spritesheet);
}

SDL_Texture* Renderer::LoadSpritemap(const std::string path) const
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


void Renderer::ExitWithSDLError(const std::string errorMessage) const
{
	std::cout << errorMessage << ": " << SDL_GetError() << std::endl;
	system("pause");
}

void Renderer::Clear()
{
	SDL_RenderClear(renderer);
}

void Renderer::Flip()
{
	SDL_RenderPresent(renderer);
}