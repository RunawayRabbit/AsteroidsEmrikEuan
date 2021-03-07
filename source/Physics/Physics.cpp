
#include "Physics.h"

#include "..\Math\Math.h"
#include "..\Math\OBB.h"

#include "ColliderType.h"
#include "CollisionTests.h"



Physics::Physics(TransformManager& transformManager, const AABB& screenAABB) :
	transformManager(transformManager),
	screenAABB(screenAABB)
{}

void Physics::RegisterPlayerShip(const Entity& playerShip)
{
	this->playerShip = playerShip;
}

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
		DetectCollisions(deltaTime);

		// Resolve all detected collisions, generate a revised moveList.
		ResolveMoves(deltaTime);
	}
}

void Physics::Begin()
{

}

void Physics::DetectCollisions(float deltaTime)
{

	auto firstLarge = moveList.begin();
	auto firstMedium = moveList.end();
	auto firstSmall = moveList.end(); // @TODO: Yea..
	auto endOfAsteroids = moveList.end();

#pragma region ShipToAsteroid

	Transform playerTrans;
	transformManager.Get(playerShip, &playerTrans);
	OBB playerOBB(playerTrans.pos, ColliderRadius::Ship, playerTrans.rot);

	for (auto largeAsteroid = firstLarge;
		largeAsteroid < firstMedium;
		++largeAsteroid)
	{
		Circle asteroid(largeAsteroid->transform.pos, ColliderRadius::Large);
		if (CollisionTests::OBBToCircle(playerOBB, asteroid))
		{
			std::cout << "Player Be Ded." << std::endl;
		}
	}

	for (auto mediumAsteroid = firstMedium;
		mediumAsteroid < firstSmall;
		++mediumAsteroid)
	{
		Circle asteroid(mediumAsteroid->transform.pos, ColliderRadius::Medium);
		if (CollisionTests::OBBToCircle(playerOBB, asteroid))
		{
			std::cout << "Player Be Ded." << std::endl;
		}
	}

	for (auto smallAsteroid = firstSmall;
		smallAsteroid < endOfAsteroids;
		++smallAsteroid)
	{
		Circle asteroid(smallAsteroid->transform.pos, ColliderRadius::Small);
		if (CollisionTests::OBBToCircle(playerOBB, asteroid))
		{
			std::cout << "Player Be Ded." << std::endl;
		}
	}

#pragma endregion

#pragma region LargeVsAll
	for (auto asteroidA = firstLarge;
		asteroidA < firstMedium - 1;
		++asteroidA)
	{
		// Test all Large vs all other Large
		for (auto asteroidB = asteroidA + 1;
			asteroidB < firstMedium;
			++asteroidB)
		{
			constexpr float largeVsLargeSqRadius = (ColliderRadius::Large + ColliderRadius::Large) *
				(ColliderRadius::Large + ColliderRadius::Large);

			float timeOfCollision;
			if (CollisionTests::SweptCircleToCircle(asteroidA->transform.pos, asteroidA->Velocity,
				asteroidB->transform.pos, asteroidB->Velocity, largeVsLargeSqRadius, deltaTime, &timeOfCollision))
			{
				//std::cout << "Collision between " << asteroidA->id.ToString() << " and " << asteroidB->id.ToString() << "!\n";
			}
		}

		// Test all Large vs all Medium
		for (auto asteroidB = firstMedium;
			asteroidB < firstSmall;
			++asteroidB)
		{
			constexpr float largeVsMediumSqRadius = (ColliderRadius::Large + ColliderRadius::Medium) *
				(ColliderRadius::Large + ColliderRadius::Medium);

			float timeOfCollision;
			if (CollisionTests::SweptCircleToCircle(asteroidA->transform.pos, asteroidA->Velocity,
				asteroidB->transform.pos, asteroidB->Velocity, largeVsMediumSqRadius, deltaTime, &timeOfCollision))
			{
				//std::cout << "Collision between " << asteroidA->id.ToString() << " and " << asteroidB->id.ToString() << "!\n";
			}
		}

		// Test all Large vs all Small
		for (auto asteroidB = firstSmall;
			asteroidB < endOfAsteroids;
			++asteroidB)
		{
			constexpr float largeVsSmallSqRadius = (ColliderRadius::Large + ColliderRadius::Small) *
				(ColliderRadius::Large + ColliderRadius::Small);

			float timeOfCollision;
			if (CollisionTests::SweptCircleToCircle(asteroidA->transform.pos, asteroidA->Velocity,
				asteroidB->transform.pos, asteroidB->Velocity, largeVsSmallSqRadius, deltaTime, &timeOfCollision))
			{
				//std::cout << "Collision between " << asteroidA->id.ToString() << " and " << asteroidB->id.ToString() << "!\n";
			}
		}
	}

#pragma endregion

#pragma region Medium_vs_All
	for (auto asteroidA = firstMedium;
		asteroidA < firstSmall - 1;
		++asteroidA)
	{

		// Test all Medium vs all other Medium
		for (auto asteroidB = asteroidA + 1;
			asteroidB < firstSmall;
			++asteroidB)
		{
			constexpr float  mediumVsMediumSqRadius = (ColliderRadius::Medium + ColliderRadius::Medium) *
				(ColliderRadius::Medium + ColliderRadius::Medium);

			float timeOfCollision;
			if (CollisionTests::SweptCircleToCircle(asteroidA->transform.pos, asteroidA->Velocity,
				asteroidB->transform.pos, asteroidB->Velocity, mediumVsMediumSqRadius, deltaTime, &timeOfCollision))
			{
				//std::cout << "Collision between " << asteroidA->id.ToString() << " and " << asteroidB->id.ToString() << "!\n";
			}
		}

		// Test all Medium vs all Small
		for (auto asteroidB = firstSmall;
			asteroidB < endOfAsteroids;
			++asteroidB)
		{
			constexpr float  mediumVsSmallSqRadius = (ColliderRadius::Medium + ColliderRadius::Small) *
				(ColliderRadius::Medium + ColliderRadius::Small);

			float timeOfCollision;
			if (CollisionTests::SweptCircleToCircle(asteroidA->transform.pos, asteroidA->Velocity,
				asteroidB->transform.pos, asteroidB->Velocity, mediumVsSmallSqRadius, deltaTime, &timeOfCollision))
			{
				//std::cout << "Collision between " << asteroidA->id.ToString() << " and " << asteroidB->id.ToString() << "!\n";
			}
		}
	}
#pragma endregion

#pragma region Small_vs_Small
	for (auto asteroidA = firstSmall;
		asteroidA < endOfAsteroids - 1;
		++asteroidA)
	{

		// Test all Small vs all other Small
		for (auto asteroidB = asteroidA + 1;
			asteroidB < endOfAsteroids;
			++asteroidB)
		{
			constexpr float smallVsSmallSqRadius = (ColliderRadius::Small + ColliderRadius::Small) *
				(ColliderRadius::Small + ColliderRadius::Small);

			float timeOfCollision;
			if (CollisionTests::SweptCircleToCircle(asteroidA->transform.pos, asteroidA->Velocity,
				asteroidB->transform.pos, asteroidB->Velocity, smallVsSmallSqRadius, deltaTime, &timeOfCollision))
			{
				//std::cout << "Collision between " << asteroidA->id.ToString() << " and " << asteroidB->id.ToString() << "!\n";
			}
		}
	}
#pragma endregion
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