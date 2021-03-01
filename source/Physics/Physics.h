#pragma once

#include <vector>

#include "..\ECS\ColliderManager.h"

#include "MoveList.h"
#include "ResolvedCollisions.h"

class Physics
{
public:
	Physics(const ColliderManager& colliderManager);

	void Simulate(MoveList& moveList, const float deltaTime);
	const std::vector<ResolvedCollisions> CollisionList() const;

private:
	struct SweptCollider
	{

	};

	static const int MAX_ITERATIONS = 3;

	void ResetForNextFrame();
	void SweepColliders(const MoveList& moveList, const float deltaTime);
	void DetectCollisions();
	void ResolveCollisions(MoveList& moveList);

	const ColliderManager& colliderManager;

	std::vector<SweptCollider> sweptColliders;
	std::vector<ResolvedCollisions> collisions;
};