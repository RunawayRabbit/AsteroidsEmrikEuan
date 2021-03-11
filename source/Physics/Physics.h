#pragma once

#include <vector>
#include <set>
#include <map>

#include "..\Math\AABB.h"
#include "ColliderType.h"

#include "..\ECS\Entity.h"
#include "..\ECS\Transform.h"

class TransformManager;
class RigidbodyManager;
struct Rigidbody;

class Physics
{
public:
	Physics(TransformManager& transformManager, RigidbodyManager& rigidbodyManager, const AABB& screenAABB);

	void RegisterPlayerShip(const Entity& playerShip);
	void Enqueue(const Entity& entity);
	void Simulate(const float& deltaTime);

private:

	struct ResolvedListEntry
	{
		Entity entity;
		Vector2 position;
		Vector2 velocity;
		float angularVelocity;
	};

	struct CollisionListEntry
	{
		Entity A;
		Entity B;
		float massA;
		float massB;
		float timeOfCollision;

		bool operator<(const CollisionListEntry& other) const
		{
			return (this->timeOfCollision < other.timeOfCollision);
		}
	};

	void DetectInitialCollisions(const float deltaTime);
	void DetectSecondaryCollisions();
	void ResolveUpdatedMovement(const float deltaTime);
	void ResolveMove(const float deltaTime, CollisionListEntry collision);
	void FinalizeMoves(const float deltaTime);
	void End();

	static constexpr int MaxSolverIterations = 3;
	static constexpr float AsteroidMasses[]{ 16.0f, 4.0f, 1.0f };

	TransformManager& transformManager;
	RigidbodyManager& rigidbodyManager;

	const AABB screenAABB;
	Entity playerShip;

	std::array<int, 3> colliderCounts;

	std::vector<Rigidbody> moveList;

	std::vector<CollisionListEntry> collisionList;
	std::multimap<float, ResolvedListEntry> resolvedList;
	std::set<Entity> dirtyList;
};