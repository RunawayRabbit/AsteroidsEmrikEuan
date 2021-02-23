#pragma once

#include <string>
#include <vector>
#include "Renderer.h"
#include "Sprite.h"

class Game
{
public:
	Game(std::string windowName, int width, int height);
	Game() = delete;
	Game(Game&) = delete;

	~Game();

	bool IsRunning() const;

	void ProcessInput();
	void Update();
	void Render();
	void Quit();

private:

	void InitSprites();

	Sprite* ship;
	Sprite* asteroid;

	std::vector<Vector2> asteroidPositions;

	Renderer renderer;

	bool isRunning;
};