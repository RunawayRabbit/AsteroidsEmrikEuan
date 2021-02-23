
#include <SDL.h>
#include <iostream> // for error messages

#include "Sprite.h"

#include "Game.h"

Game::Game(std::string windowName, int width, int height) :
	renderer(windowName, width, height),
	spriteSet(renderer),
	isRunning(true)
{

	
	playerShip = std::make_unique<Ship>(Ship(spriteSet, { 100.0f, 200.0f }, 0.0f));

	for (size_t i = 0; i < 5; i++)
	{
		asteroids.push_back(std::make_unique<Asteroid>(Asteroid(spriteSet, { 400.0f, 300.0f }, 0.0f)));
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
	playerShip->Update(deltaTime);

	for (auto& asteroid : asteroids)
	{
		asteroid->Update(deltaTime);
	}
}


void Game::Render()
{
	renderer.Clear();

	playerShip->Draw();

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
