#pragma once

#include <vector>
#include <set>
#include <map>
#include <future>

#include "..\Math\AABB.h"

#include "..\ECS\Entity.h"
#include "..\ECS\Transform.h"
#include "..\ECS\Rigidbody.h"

#include "ColliderType.h"
#include "MoveList.h"

class TransformManager;
class RigidbodyManager;

class Physics
{
public:
	Physics(TransformManager& transformManager, RigidbodyManager& rigidbodyManager, const AABB& screenAABB);

	void Enqueue(const Rigidbody& rb, const float& deltaTime);
	void Simulate(const float& deltaTime);

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

	const std::vector<CollisionListEntry>& GetCollisions(){ return collisionList; };


private:

	struct ResolvedListEntry
	{
		Entity entity;
		Vector2 position;
		Vector2 velocity;
		float angularVelocity = 0;
		float time = 0;
	};

	std::vector<Physics::CollisionListEntry> DetectInitialCollisions(MoveList& moveList, const float& deltaTime);
	void DetectSecondaryCollisions(const std::vector<Physics::ResolvedListEntry> resolvedThisIteration);
	std::vector<Physics::ResolvedListEntry> ResolveUpdatedMovement(const float& deltaTime);
	std::array<Physics::ResolvedListEntry, 2> ResolveMove(const float& deltaTime, CollisionListEntry collision);
	void FinalizeMoves(const float& deltaTime);
	void End();

	void ShipVsAsteroid(const MoveList::ColliderRanges& ranges, std::vector<CollisionListEntry>& collisions);
	void OBBVsSpecificAsteroid(const OBB& ship, std::vector<MoveList::Entry>::iterator asteroidBegin,
		std::vector<MoveList::Entry>::iterator asteroidEnd, const float& asteroidRadius);

	void BulletVsAsteroid(const MoveList::ColliderRanges& ranges, std::vector<CollisionListEntry>& collisions, const float& deltaTime);
	void AsteroidVsAsteroid(const MoveList::ColliderRanges& ranges, std::vector<CollisionListEntry>& collisions, const float& deltaTime);
	void CircleVsCircles(const MoveList::Entry& circle, const float& circleRadius, const float& circleMass,
		std::vector<MoveList::Entry>::iterator circlesBegin, std::vector<MoveList::Entry>::iterator circlesEnd,
		const float& circlesMass, const float& radiusPlusRadiusSquared, const float& deltaTime,
		std::vector<CollisionListEntry>& collisions);

	static constexpr int MaxSolverIterations = 3;
	static constexpr float AsteroidMasses[]{ 16.0f, 4.0f, 1.0f };

	TransformManager& transformManager;
	RigidbodyManager& rigidbodyManager;

	const AABB screenAABB;

	static constexpr int chunksX = 8;
	static constexpr int chunksY = 6;
	static constexpr int chunkCount = chunksX * chunksY;

	const float chunkSizeX;
	const float chunkSizeY;

	std::array<MoveList, chunkCount> moveLists;
	std::array<std::future<std::vector<CollisionListEntry>>, chunkCount> workers;

	std::vector<CollisionListEntry> collisionList;
	std::vector<ResolvedListEntry> resolvedList;
	std::set<Entity> dirtyList;
};
