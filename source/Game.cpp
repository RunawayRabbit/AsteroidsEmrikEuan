
#include <SDL.h>
#include <iostream> // for error messages

#include "Sprite.h";

#include "Game.h"

Game::Game(std::string windowName, int width, int height) :
	renderer(windowName, width, height),
	isRunning(true)
{
	InitSprites();
}

void Game::InitSprites()
{
	SDL_Rect shipSrc;
	shipSrc.w = 22;
	shipSrc.h = 30;
	shipSrc.x = 5;
	shipSrc.y = 1;

	ship = new Sprite(renderer, shipSrc);

	SDL_Rect asteroidSrc;
	asteroidSrc.w = 58;
	asteroidSrc.h = 61;
	asteroidSrc.x = 66;
	asteroidSrc.y = 194;
	asteroid = new Sprite(renderer, asteroidSrc);
}

Game::~Game()
{
	delete ship;
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
	if (asteroidPositions.size() == 0)
	{
		asteroidPositions.push_back({ 150,100 });
		asteroidPositions.push_back({ 400,150 });
		asteroidPositions.push_back({ 170,300 });
		asteroidPositions.push_back({ 700,460 });
	}
}


void Game::Render()
{
	renderer.Clear();

	Vector2 shipPosition;
	shipPosition.x = 100;
	shipPosition.y = 200;
	ship->Draw(shipPosition, 0.0f);

	for (auto& pos : asteroidPositions)
	{
		asteroid->Draw(pos, 0.0f);
	}

	renderer.Flip();

}

void Game::Quit()
{
	isRunning = false;
}
