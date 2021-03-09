
#include <SDL.h>
#include <iostream> // for error messages

#include <functional>

#include "..\GameObject\Create.h"

#include "..\Math\Math.h"
#include "..\Math\Circle.h"

#include "..\Physics\Physics.h"

#include "Game.h"


static Entity GLOBALtestEntity;


Game::Game(std::string windowName, int width, int height) :
	renderer(windowName, width, height),
	input(InputHandler(*this)),
	gameField(0.0f, (float)height, 0.0f, (float)width),
	xforms(2), //intial capacity. Can resize dynamically.
	sprites(renderer, xforms, entities, AABB(0, (float)height, 0, (float)width), 128),
	rigidbodies(entities, 2),
	create(entities, xforms, sprites, rigidbodies),
	physics(xforms, rigidbodies, AABB(0, (float)height, 0, (float)width)),
	GCStep(0),
	isRunning(true)
{
#if 1
	GLOBALtestEntity = create.Ship(Vector2::zero(), 0);

	physics.RegisterPlayerShip(GLOBALtestEntity);

	std::vector<Vector2> asteroidPositions;

	for (auto i = 0; i < 64; i++)
	{
		bool isValidPosition = false;
		int attempts = 0;
		while (!isValidPosition)
		{
			if (attempts > 32)
			{
				// Yea I know. It's temporary, ok?
				return;
			}
			constexpr float asteroidRadius = 29;
			Vector2 startPos = Vector2{ Math::RandomRange(asteroidRadius, width - asteroidRadius),
				Math::RandomRange(asteroidRadius, height - asteroidRadius) };

			isValidPosition = true;

			Circle newAsteroid(startPos, asteroidRadius);
			for (auto& existingAsteroid : asteroidPositions)
			{
				Circle existing(existingAsteroid, asteroidRadius + 5.0f);
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

				Entity entity = entities.Create();

				Transform trans;
				trans.pos = startPos;
				trans.rot = startRot;
				xforms.Add(entity, trans);

				sprites.Create(entity, SpriteID::LARGE_ASTEROID, RenderQueue::Layer::DEFAULT);
				rigidbodies.Add(entity, ColliderType::LARGE_ASTEROID, startVel, rotVel);
			}
			++attempts;
		}
	}
#else


	create.Asteroid(Vector2{ (float)width - 340, (float)(height / 4) - 15 }, 0, {-30.0f, 0.0f}, 0);


	//Entity entity = entities.Create();

	//Transform trans;
	//trans.pos = {width-340.0f, height*0.6f};
	//trans.rot = 0;
	//xforms.Add(entity, trans);
	//sprites.Create(entity, SpriteID::MEDIUM_ASTEROID_1, RenderQueue::Layer::DEFAULT);
	//rigidbodies.Add(entity, ColliderType::MEDIUM_ASTEROID, Vector2::zero(), 0);

	create.Asteroid(Vector2{ (float)0 + 340, (float)(height / 4) + 15 }, 0, { 30.0f, 0.0f }, 0);

	GLOBALtestEntity = create.Ship(Vector2::zero(), 0);
	physics.RegisterPlayerShip(GLOBALtestEntity);

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
	input.ProcessInput();
}

void Game::Update(float deltaTime)
{

	/* FREEWRITE ON UPDATE
	
	We start by setting up something called a "MoveList". This is going to be a list of everything
	that tried to move this frame. Contains..
		
	EntityID
	Velocity
	(AngularVelocity?)
	Collider
	TransformID.

	The act of enqueueing actually pulls the Collider out, and places it at the bottom of the
	ColliderManager's list of colliders. This is important for later on, as if we do this,
	ColliderManager's internal list is now sorted with static elements first, moving elements last.
	We know the position of the first moving element by taking ColliderManagersize - MoveList.size.
	*/

	/// GAMEPLAY CODE GOES HERE.

	auto inputBuffer = input.GetBuffer();

	rigidbodies.Update(physics, deltaTime);
		
	Transform transform;
	transform.pos = Vector2{ (float)inputBuffer.mouseX, (float)inputBuffer.mouseY };

	transform.rot = 0;
	xforms.Set(GLOBALtestEntity, transform);


	/*

	Once MoveList has been created/cleared/whatever, we do our individual updates on gameplay-level
	components. The Player component pulls in inputs and enqueues itself to MoveList, Asteroids
	enqueue themselves with their current velocities, bullets as well, and so on.
	
	// @TODO: Think about how having different kinds of colliders will screw with this. What happens
		if colliders rotate, how do we make that work?
	
	// @TODO: Seems more and more intelligent to have Collider and Transform share sorting order,
		or perhaps even be rolled together into a ColliderTransform for processing at least?


	// Pass moveList in by reference so we can call Enqueue.
	player.Update(moveList, previousFrameCollisions, deltaTime);
	
	for (Asteroid& asteroid : rigidbodies)
	{
		asteroid.Update(moveList, previousFrameCollisions, deltaTime);
	}

	*/

	physics.Simulate(deltaTime);
	
	// @TODO: We refresh the collision list here. We can cache it for next frame.
	//std::vector<Entity> dirtyList = xforms.Update(physics.DirtyList());

	/*

	We can now update the Transform list, so we get a lock on it and begin burning through our
	ResolvedCollisions, finding their transforms and applying the motion to them. This is the first
	time that our physics system has mutated global state!

	As a final step, we save ResolvedCollisions into a table that the gameplay components can query
	on the next frame to see what they collided with, or if they collided at all.
	
	*/

	sprites.Update(deltaTime);

	// GC

	GarbageCollection();
}

void Game::Render()
{
	// Prepare the render queue for the renderer
	renderQueue.Clear();

	// @TODO: Render all static backgrounds
	//renderQueue.Enqueue()


	sprites.Render(renderQueue);


	// Pass the render queue to the renderer
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
