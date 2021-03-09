#pragma once

#include <string>
#include <vector>

#include "../Renderer/Renderer.h"
#include "../Renderer/RenderQueue.h"

#include "../ECS/EntityManager.h"
#include "../ECS/TransformManager.h"
#include "../ECS/AnimatedSpriteManager.h"
#include "../ECS/SpriteManager.h"
#include "../ECS/RigidbodyManager.h"

#include "../GameObject/Create.h"

#include "../Physics/Physics.h"

#include "../Input/InputHandler.h"

#include "../Math/AABB.h"


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

	void GarbageCollection();

private:
	Renderer renderer;
	RenderQueue renderQueue;

	InputHandler input;

	// GameObject Creator
	Create create;

	// ECS Systems
	EntityManager entities;
	TransformManager xforms;
	SpriteManager sprites;

	RigidbodyManager rigidbodies;

	Physics physics;
	const AABB gameField;

	//std::unique_ptr<Ship> playerShip;

	int GCStep;

	bool isRunning;
};