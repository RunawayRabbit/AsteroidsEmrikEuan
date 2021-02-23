
#include <SDL.h>
#include <iostream> // for error messages

#include "SpriteAtlas.h"

#include "Game.h"

Game::Game(std::string windowName, int width, int height) :
	renderer(windowName, width, height),
	spriteAtlas(renderer),
	isRunning(true),
	gameField(0.0f, (float)height, 0.0f, (float)width)
{
	
	playerShip = std::make_unique<Ship>(Ship(spriteAtlas, &gameField, { 100.0f, 200.0f }, 0.0f));

	for (size_t i = 0; i < 5; i++)
	{
		asteroids.push_back(std::make_unique<Asteroid>(Asteroid(spriteAtlas, &gameField, { 400.0f, 300.0f }, 0.0f)));
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
	
	static int frameCount = 0;
	static int animTest = 0;
	frameCount++;
	if (frameCount % 120) animTest++;

	spriteAtlas.shipTrailSprite->Draw({ 300, 300 }, 0.0f, animTest % 4);

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
