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

	void DetectInitialCollisions(const float& deltaTime);
	void DetectSecondaryCollisions();
	void ResolveUpdatedMovement(const float& deltaTime);
	void ResolveMove(const float& deltaTime, CollisionListEntry collision);
	void FinalizeMoves(const float& deltaTime);
	void End();

	struct ColliderRanges
	{
		std::vector<Rigidbody>::iterator shipBegin;
		std::vector<Rigidbody>::iterator shipEnd;
		std::vector<Rigidbody>::iterator bulletBegin;
		std::vector<Rigidbody>::iterator bulletEnd;
		std::vector<Rigidbody>::iterator largeBegin;
		std::vector<Rigidbody>::iterator largeEnd;
		std::vector<Rigidbody>::iterator mediumBegin;
		std::vector<Rigidbody>::iterator mediumEnd;
		std::vector<Rigidbody>::iterator smallBegin;
		std::vector<Rigidbody>::iterator smallEnd;
	};

	void ShipVsAsteroid(const ColliderRanges& ranges);
	void OBBVsSpecificAsteroid(const OBB& ship, std::vector<Rigidbody>::iterator asteroidBegin,
		std::vector<Rigidbody>::iterator asteroidEnd, const float& asteroidRadius);

	void BulletVsAsteroid(const ColliderRanges& ranges, const float& deltaTime);
	void AsteroidVsAsteroid(const ColliderRanges& ranges, const float& deltaTime);
	void CircleVsCircles(const Rigidbody& circle, const float& circleRadius, const float& circleMass,
		std::vector<Rigidbody>::iterator circlesBegin, std::vector<Rigidbody>::iterator circlesEnd,
		const float& circlesMass, const float& circlesRadii, const float& deltaTime);

	static constexpr int MaxSolverIterations = 3;
	static constexpr float AsteroidMasses[]{ 16.0f, 4.0f, 1.0f };

	TransformManager& transformManager;
	RigidbodyManager& rigidbodyManager;

	const AABB screenAABB;
	Entity playerShip;

	std::array<int, (int)ColliderType::COUNT> colliderCounts;

	std::vector<Rigidbody> moveList;

	std::vector<CollisionListEntry> collisionList;
	std::multimap<float, ResolvedListEntry> resolvedList;
	std::set<Entity> dirtyList;
};