
#include <SDL.h>
#include <iostream> // for error messages


#include "..\ECS\Entity.h"
#include "..\ECS\Transform.h"
#include "..\Physics\Physics.h"

#include "Game.h"



Game::Game(std::string windowName, int width, int height) :
	renderer(windowName, width, height),
	input(InputHandler(*this)),
	spriteAtlas(renderer),
	isRunning(true),
	gameField(0.0f, (float)height, 0.0f, (float)width),
	xforms(512), //intial capacity. Can resize dynamically.
	colliders(),
	physics(colliders)
{



	// Instantiate Game Objects
	/*
	playerShip = std::make_unique<Ship>(Ship(spriteAtlas, &gameField, { 100.0f, 200.0f }, 0.0f));

	for (size_t i = 0; i < 5; i++)
	{
		asteroids.push_back(std::make_unique<Asteroid>(Asteroid(spriteAtlas, &gameField, { 400.0f, 300.0f }, 0.0f)));
	}*/
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


	MoveList moveList;
	moveList.Clear();

	/*

	Once MoveList has been created/cleared/whatever, we do our individual updates on gameplay-level
	components. The Player component pulls in inputs and enqueues itself to MoveList, Asteroids
	enqueue themselves with their current velocities, bullets as well, and so on.
	
	// @TODO: Think about how having different kinds of colliders will screw with this. What happens
		if colliders rotate, how do we make that work?
	
	// @TODO: Seems more and more intelligent to have Collider and Transform share sorting order,
		or perhaps even be rolled together into a ColliderTransform for processing at least?
	
	*/

	auto previousFrameCollisions = physics.CollisionList();

	/*
	// Pass moveList in by reference so we can call Enqueue.
	player.Update(moveList, previousFrameCollisions, deltaTime);
	
	for (Asteroid& asteroid : asteroids)
	{
		asteroid.Update(moveList, previousFrameCollisions, deltaTime);
	}

	*/
	animSprites.Update(deltaTime);





	physics.Simulate(moveList, deltaTime);
	
	// @TODO: We refresh the collision list here. We can cache it for next frame.
	xforms.Update(physics.CollisionList());

	/*

	We can now update the Transform list, so we get a lock on it and begin burning through our
	ResolvedCollisions, finding their transforms and applying the motion to them. This is the first
	time that our physics system has mutated global state!

	As a final step, we save ResolvedCollisions into a table that the gameplay components can query
	on the next frame to see what they collided with, or if they collided at all.
	
	*/

}

void Game::Render()
{
	// Prepare the render queue for the renderer
	renderQueue.Clear();

	/*
	
	Step 1, AnimatedSprites need to select the correct frame and adjust their
	entry in the SpriteManager to reflect this change. AnimatedSprites should
	be sorted to the front of SpriteManager to make this smoother.
	*/


	/*
	Step 1.5, Static elements such as the background should just go ahead and
	push themselvse onto the renderQueue now. No reason to wait, and we would
	prefer to have them sorted lower.

	*/

	// Render all static backgrounds
	//renderQueue.Enqueue()

	sprites.Render(renderQueue);
	//UIManager.Render();


	// Pass the render queue to the renderer
	renderer.Render(renderQueue.GetRenderQueue());
}

void Game::Quit()
{
	isRunning = false;
}
