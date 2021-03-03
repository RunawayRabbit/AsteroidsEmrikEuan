#pragma once

#include <vector>

#include "..\ECS\ColliderManager.h"

#include "MoveList.h"
#include "Move.h"

class Physics
{
public:
	Physics(const ColliderManager& colliderManager);

	void Simulate(MoveList& moveList, const float deltaTime);
	const std::vector<Move> DirtyList() const;

private:
	struct SweptCollider
	{

	};

	static const int MAX_ITERATIONS = 3;

	void ResetForNextFrame();
	void SweepColliders(const MoveList& moveList, const float deltaTime);
	void DetectCollisions();
	void ResolveMoves();

	const ColliderManager& colliderManager;

	std::vector<SweptCollider> sweptColliders;
	std::vector<Move> finalizedMoves;
};