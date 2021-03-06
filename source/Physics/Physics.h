#pragma once

#include <vector>
#include <functional>

#include "MoveList.h"
#include "Move.h"

#include "..\Math\AABB.h"

#include "ColliderType.h"

constexpr float largeAsteroidRadius = 27; // ?
constexpr float mediumAsteroidRadius = 12; // ?
constexpr float smallAsteroidRadius = 6;

class Physics
{
public:
	Physics(TransformManager& transformManager, const AABB& screenAABB);

	void Reset();

	void Enqueue(const Entity& entity, const Vector2& velocity, const float& angularVelocity);

	void Simulate(Entity testA, Entity testB, const float deltaTime);
	const std::vector<Move> DirtyList() const;



private:

	static constexpr int MAX_ITERATIONS = 3;

	void Begin();
	void SweepColliders(const float deltaTime);
	void DetectCollisions(Entity testA, Entity testB, float deltaTime);
	void ResolveMoves(const float deltaTime);

	std::vector<Move> finalizedMoves;

	TransformManager& transformManager;

	const AABB screenAABB;
	// Move List Stuff

	struct Entry
	{
		Entity id;
		Vector2 Velocity;
		float angularVelocity;
		Transform transform; // initialize this by copying from the entityID in the xform manager
	};

	std::vector<Entry> moveList;
};