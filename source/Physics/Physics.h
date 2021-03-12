#pragma once

#include <vector>
#include <set>
#include <map>

#include "..\Math\AABB.h"
#include "ColliderType.h"

#include "..\ECS\Entity.h"
#include "..\ECS\Transform.h"
#include "..\ECS\Rigidbody.h"

class TransformManager;
class RigidbodyManager;

class Physics
{
public:
	Physics(TransformManager& transformManager, RigidbodyManager& rigidbodyManager, const AABB& screenAABB);

	void Enqueue(const Rigidbody& rb);
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

	const std::vector<CollisionListEntry>& GetCollisions()
	{
		return collisionList;
	};

	// Kinda mad that this has to be public. But it's the only way I know of
	// making it visible so unordered_set can access it's hash function.
	struct MoveListEntry
	{
		Rigidbody rb;
		Vector2 pos;

		bool operator==(const MoveListEntry& other) const
		{
			return (rb.entity == other.rb.entity);
		}
	};

private:

	struct ResolvedListEntry
	{
		Entity entity;
		Vector2 position;
		Vector2 velocity;
		float angularVelocity = 0;
	};

	void DetectInitialCollisions(const float& deltaTime);
	void DetectSecondaryCollisions();
	void ResolveUpdatedMovement(const float& deltaTime);
	void ResolveMove(const float& deltaTime, CollisionListEntry collision);
	void FinalizeMoves(const float& deltaTime);
	void End();

	struct ColliderRanges
	{
		std::vector<MoveListEntry>::iterator shipBegin;
		std::vector<MoveListEntry>::iterator shipEnd;
		std::vector<MoveListEntry>::iterator bulletBegin;
		std::vector<MoveListEntry>::iterator bulletEnd;
		std::vector<MoveListEntry>::iterator largeBegin;
		std::vector<MoveListEntry>::iterator largeEnd;
		std::vector<MoveListEntry>::iterator mediumBegin;
		std::vector<MoveListEntry>::iterator mediumEnd;
		std::vector<MoveListEntry>::iterator smallBegin;
		std::vector<MoveListEntry>::iterator smallEnd;
	};

	void ShipVsAsteroid(const ColliderRanges& ranges);
	void OBBVsSpecificAsteroid(const OBB& ship, std::vector<MoveListEntry>::iterator asteroidBegin,
		std::vector<MoveListEntry>::iterator asteroidEnd, const float& asteroidRadius);

	void BulletVsAsteroid(const ColliderRanges& ranges, const float& deltaTime);
	void AsteroidVsAsteroid(const ColliderRanges& ranges, const float& deltaTime);
	void CircleVsCircles(const MoveListEntry& circle, const float& circleRadius, const float& circleMass,
		std::vector<MoveListEntry>::iterator circlesBegin, std::vector<MoveListEntry>::iterator circlesEnd,
		const float& circlesMass, const float& circlesRadii, const float& deltaTime);

	static constexpr int MaxSolverIterations = 3;
	static constexpr float AsteroidMasses[]{ 16.0f, 4.0f, 1.0f };

	TransformManager& transformManager;
	RigidbodyManager& rigidbodyManager;

	const AABB screenAABB;

	std::array<int, (int)ColliderType::COUNT> colliderCounts;

	std::vector<MoveListEntry> moveList;

	std::vector<CollisionListEntry> collisionList;
	std::multimap<float, ResolvedListEntry> resolvedList;
	std::set<Entity> dirtyList;
};

namespace std {
	template<>
	struct hash<Physics::MoveListEntry> {
		inline size_t operator()(const Physics::MoveListEntry& moveListEntry) const {
			return moveListEntry.rb.entity.hash();
		}
	};
}