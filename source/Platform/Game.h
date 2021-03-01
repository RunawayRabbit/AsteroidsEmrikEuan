#pragma once

#include <string>
#include <vector>

#include "../Renderer/Renderer.h"
#include "../Renderer/SpriteAtlas.h"
#include "../Renderer/RenderQueue.h"

#include "../ECS/EntityManager.h"
#include "../ECS/TransformManager.h"
#include "../ECS/ColliderManager.h"
#include "../ECS/AnimatedSpriteManager.h"
#include "../ECS/SpriteManager.h"

#include "../Physics/Physics.h"

#include "../GameObject/Asteroid.h"
#include "../GameObject/Ship.h"

#include "../Input/InputHandler.h"

#include "../Math/Rect.h"


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
	RenderQueue renderQueue;
	SpriteAtlas spriteAtlas; // @TODO: Move this to the renderer?

	InputHandler input;

	// ECS Systems
	EntityManager entities;
	TransformManager xforms;
	ColliderManager colliders;

	// @TODO: Is this ECS business, Renderer business or an entirely new thing?
	AnimatedSpriteManager animSprites;
	SpriteManager sprites;

	Physics physics;
	const Rect gameField;

	//std::unique_ptr<Ship> playerShip;
	//std::vector<std::unique_ptr<Asteroid>> asteroids;


	bool isRunning;
};