#pragma once

#include <string>
#include <vector>
#include "Renderer.h"
#include "Sprite.h"
#include "Asteroid.h"


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

	std::vector<std::unique_ptr<Entity>> asteroids;


	bool isRunning;
};