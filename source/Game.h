#pragma once

#include <string>
#include <vector>
#include "Renderer.h"
#include "Sprite.h"
#include "Asteroid.h"
#include "Ship.h"


class Game
{
public:
	Game(std::string windowName, int width, int height);
	Game() = delete;
	Game(Game&) = delete;

	~Game();

	bool IsRunning() const;

	void ProcessInput();
	void Update(float deltaTime);
	void Render();
	void Quit();

private:
	Renderer renderer;
	SpriteSet spriteSet;

	const Rect gameField;

	std::unique_ptr<Ship> playerShip;
	std::vector<std::unique_ptr<Asteroid>> asteroids;


	bool isRunning;
};