
#include <SDL.h>
#include <iostream> // for error messages

#include "Sprite.h"

#include "Game.h"

Game::Game(std::string windowName, int width, int height) :
	renderer(windowName, width, height),
	spriteSet(renderer),
	isRunning(true)
{

	for (size_t i = 0; i < 5; i++)
	{
		asteroids.push_back(Entity::Make(Entity::Type::Asteroid, spriteSet, { 400.0f, 300.0f }, 0.0f));
	}
}

Game::~Game()
{
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

void Game::Update(float deltaTime)
{
	for (auto& asteroid : asteroids)
	{
		asteroid->Update(deltaTime);
	}
}


void Game::Render()
{
	renderer.Clear();

	Vector2 shipPosition;
	shipPosition.x = 100;
	shipPosition.y = 200;
	//ship->Draw(shipPosition, 0.0f);

	for (auto& asteroid : asteroids)
	{
		asteroid->Draw();
	}

	renderer.Flip();
}

void Game::Quit()
{
	isRunning = false;
}
