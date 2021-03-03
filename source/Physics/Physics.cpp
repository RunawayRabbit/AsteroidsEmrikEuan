
#include "Physics.h"
#include "..\ECS\ColliderManager.h"

Physics::Physics(const ColliderManager& colliderManager) :
	colliderManager(colliderManager)
{

}

void Physics::Simulate(MoveList &moveList, const float deltaTime)
{
	// At this point we initialize a "ResolvedCollisions" list. We'll get to that.
	ResetForNextFrame();

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
		(i < MAX_ITERATIONS) && (moveList.Size() > 0);
		++i)
	{
		// Generate swept colliders for use in detection phase
		SweepColliders(moveList, deltaTime);

		// Test swept colliders vs each other, and against all static colliders
		DetectCollisions();

		// Resolve all detected collisions, generate a revised moveList.
		ResolveMoves();
	}
}

void Physics::ResetForNextFrame()
{

}

void Physics::SweepColliders(const MoveList& moveList, const float deltaTime)
{

}

void Physics::DetectCollisions()
{
	// First, test against resolved move list

	// Then, test against against the live colliders
	const std::vector<Collider> colliders = colliderManager.GetColliders();
}

void Physics::ResolveMoves()
{

}

const std::vector<Move> Physics::DirtyList() const
{
	return finalizedMoves;
}