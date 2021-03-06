
#include "Physics.h"

#include "..\Math\Math.h"
#include "..\Math\OBB.h"

#include "ColliderType.h"
#include "CollisionTests.h"

Physics::Physics(TransformManager& transformManager, const AABB& screenAABB) :
	transformManager(transformManager),
	screenAABB(screenAABB)
{}

void Physics::Reset()
{
	moveList.clear();
}

void Physics::Enqueue(const Entity& entity, const Vector2& velocity, const float& angularVelocity)
{
	// @TODO: Rearrange this data to make more sense to the collision detector.

	Entry entry;
	entry.id = entity;
	entry.Velocity = velocity;
	entry.angularVelocity = angularVelocity;
	transformManager.Get(entity, &entry.transform);

	moveList.push_back(entry);
}

void Physics::Simulate(Entity testA, Entity testB, const float deltaTime)
{

	// At this point we initialize a "ResolvedCollisions" list. We'll get to that.
	Begin();

	/*
	Next comes PhysicsSweep. This takes the MoveList and deltaTime and constructs a swept volume
	to create a region that	the collider will occupy over this frame.

	Next, we do a CollisionDetection. This checks all SweptColliders from the previous step against
	all other SweptColliders, and if a collision is found, it places it onto a Collision list. It
	also checks every SweptCollider against every static Collider on the ColliderManager, ie up to
	the point where the moving colliders are, but no further.

	We then take these Collisions and pass them into a CollisionResolver, which does the math on
	how the collision should be resolved. It then updates the SweptColliders to represent the
	remaining travel expected, and puts them on a fresh SweptColliders list. It also populates
	the ResolvedCollisions list with each solve it has performed.
	*/

	for (auto i = 0;
		(i < MAX_ITERATIONS) && (moveList.size() > 0);
		++i)
	{

		// Test swept colliders vs each other, and against all static colliders
		DetectCollisions(testA, testB, deltaTime);

		// Resolve all detected collisions, generate a revised moveList.
		ResolveMoves(deltaTime);
	}
}

void Physics::Begin()
{

}

void Physics::SweepColliders(const float deltaTime)
{

}


void Physics::DetectCollisions(Entity testA, Entity testB, float deltaTime)
{
	Transform transA;
	transformManager.Get(testA, &transA);

	Transform transB;
	transformManager.Get(testB, &transB);

	Circle circleA(transA.pos, ColliderRadius::Large);
	Circle circleB(transB.pos, ColliderRadius::Large);

	SDL_Rect testShip;
	testShip.x = transA.pos.x - ColliderRadius::Ship / 2.0f;
	testShip.y = transA.pos.y - ColliderRadius::Ship / 2.0f;
	testShip.w = ColliderRadius::Ship;
	testShip.h = ColliderRadius::Ship;

	OBB shipA(testShip, transA.rot);
	//std::cout << shipA.center.x << ", " << shipA.center.y << "\n";


	CollisionTests::OBBToCircle(shipA, circleB);

	/*
	float timeOfCollision;
	if (CollisionTests::SweptCircleToCircle(circleA, Vector2::zero(), circleB, Vector2{ 20.0f, 0.0f }, deltaTime, &timeOfCollision))
	{
		//This works apparently.
	}
	*/

}


void Physics::ResolveMoves(const float deltaTime)
{
	for (auto& entry : moveList)
	{
		Transform* trans;
		transformManager.GetPtr(entry.id, &trans);
		trans->pos.x =
			Math::Repeat(trans->pos.x + (entry.Velocity.x * deltaTime), screenAABB.right);
		trans->pos.y =
			Math::Repeat(trans->pos.y + (entry.Velocity.y * deltaTime), screenAABB.bottom);

		trans->rot = Math::Repeat(trans->rot + (entry.angularVelocity * deltaTime), 360.0f);
	}
}

const std::vector<Move> Physics::DirtyList() const
{
	return finalizedMoves;
}