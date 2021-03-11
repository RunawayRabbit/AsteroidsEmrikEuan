
#include <SDL.h>
#include <iostream> // for error messages

#include <functional>

#include "..\GameObject\Create.h"

#include "..\Math\Math.h"
#include "..\Math\Circle.h"

#include "..\Physics\Physics.h"

#include "Game.h"


static std::vector<Entity> GLOBALtestEntities;


Game::Game(std::string windowName, int width, int height) :
	renderer(windowName, width, height),
	input(InputHandler(*this)),
	gameField(0.0f, (float)height, 0.0f, (float)width),
	xforms(2), //intial capacity. Can resize dynamically.
	rigidbodies(entities, 2),
	sprites(renderer, xforms, entities, AABB(0, (float)height, 0, (float)width), 128),
	create(entities, xforms, sprites, rigidbodies),
	physics(xforms, rigidbodies, AABB(0, (float)height, 0, (float)width)),
	player(entities, rigidbodies, xforms, create, physics),
	GCStep(0),
	isRunning(true)
{
#if 1

	std::vector<Vector2> asteroidPositions;

	for (auto i = 0; i < 400; i++)
	{
		bool isValidPosition = false;
		int attempts = 0;
		while (!isValidPosition)
		{
			if (attempts > 1024)
			{
				// Yea I know. It's temporary, ok?
				return;
			}
			constexpr float asteroidRadius = 10;
			Vector2 startPos = Vector2{ Math::RandomRange(asteroidRadius, width - asteroidRadius),
				Math::RandomRange(asteroidRadius, height - asteroidRadius) };

			isValidPosition = true;

			Circle newAsteroid(startPos, asteroidRadius);
			for (auto& existingAsteroid : asteroidPositions)
			{
				Circle existing(existingAsteroid, asteroidRadius);
				if (existing.Overlaps(newAsteroid))
				{
					isValidPosition = false;
					break;
				}				
			}

			if (isValidPosition)
			{
				float startRot = rand() % 360;
				Vector2 startVel = Vector2{ Math::RandomRange(-40, 40), Math::RandomRange(-40, 40) };
				float rotVel = Math::RandomRange(-45, 45);

				asteroidPositions.push_back(startPos);
				
				GLOBALtestEntities.push_back(create.Asteroid(startPos, startRot, startVel, rotVel, Create::AsteroidType::RANDOM_SMALL));
			}
			++attempts;
		}
		std::cout << asteroidPositions.size() << std::endl;
	}


#endif

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
	input.Clear();
	input.ProcessInput();
}

void Game::Update(float deltaTime)
{
	auto inputBuffer = input.GetBuffer();

	// Gameplay Code Goes Here!

	if (inputBuffer.Contains(InputOneShot::Bomb))
	{
		player.Spawn(gameField.max * 0.5f, 180.0f);
	}

	player.Update(inputBuffer, deltaTime);




	rigidbodies.Update(physics, deltaTime);
	physics.Simulate(deltaTime);
	sprites.Update(deltaTime);

	GarbageCollection();
}

void Game::Render()
{
	renderQueue.Clear();

	// @TODO: Render all static backgrounds
	//renderQueue.Enqueue()

	sprites.Render(renderQueue);

	renderer.Render(renderQueue.GetRenderQueue());
}

void Game::Quit()
{
	isRunning = false;
}

void Game::GarbageCollection()
{
	// @TODO: This can be better.

	// Kind of a funky way of doing GC, will likely clean this up later.
	// Each component that does GC takes turns, one per frame.

	switch (++GCStep)
	{
	case 1:
		xforms.GarbageCollect(entities);
		break;

	default:
		GCStep = 0;
	}
}
