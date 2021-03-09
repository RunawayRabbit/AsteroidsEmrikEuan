
#include "Physics.h"
#include "..\ECS\RigidbodyManager.h"
#include "..\ECS\TransformManager.h"

#include "..\Math\Math.h"
#include "..\Math\OBB.h"

#include "ColliderType.h"
#include "CollisionTests.h"

Physics::Physics(TransformManager& transformManager, RigidbodyManager& rigidbodyManager, const AABB& screenAABB) :
	transformManager(transformManager),
	rigidbodyManager(rigidbodyManager),
	screenAABB(screenAABB)
{}

void Physics::RegisterPlayerShip(const Entity& playerShip)
{
	this->playerShip = playerShip;
}

void Physics::Enqueue(const Entity& entity)
{
	Rigidbody* rb;
	if (rigidbodyManager.GetPtr(entity, &rb))
	{
		// I really don't like doing this.
		moveList.push_back(*rb);
	}
}

void Physics::Simulate(const float& deltaTime)
{
	// I think this is robust.

	// Step 1. We have a MoveList.
	// Step 2. We detect collisions within that movelist.
	// Step 3. If we find a collision, add it into a multimap sorted on collision time, earliest first.
	DetectInitialCollisions(deltaTime);

	int solverIterations = 0;
	do
	{
		// Step 4. Start with the earliest collision and resolve the new motion and add the entities that collided to a "dirty set".
		// Step 5. Continue resolving collisions, but skip any that involve objects in the dirty set.
		std::sort(collisionList.begin(), collisionList.end());
		ResolveUpdatedMovement(deltaTime);

		// Step 6. Clear the collision list and the dirty set.
		collisionList.clear();
		dirtyList.clear();

		// Step 7. Check the NEW elements in resolved list against the full move list, looking for further collisions.
		DetectSecondaryCollisions();

		// Step 8. If the collision list is empty, continue. Otherwise, loop..
		++solverIterations;
	} while (collisionList.size() > 0 && solverIterations < MaxSolverIterations);

	FinalizeMoves(deltaTime);
	End();
}

void Physics::DetectSecondaryCollisions()
{
	// STUB
	// Let's pretend that we do this step :kappa:
}

void Physics::End()
{
	moveList.clear();
	collisionList.clear();
	resolvedList.clear();
	dirtyList.clear();
}

void Physics::DetectInitialCollisions(const float deltaTime)
{
	std::sort(moveList.begin(), moveList.end());

	auto firstLarge = moveList.begin();
	auto firstMedium = moveList.end();
	auto firstSmall = moveList.end(); // @TODO: Yea..
	auto endOfAsteroids = moveList.end();

#pragma region ShipToAsteroid

	Transform* playerTrans;
	transformManager.GetPtr(playerShip, &playerTrans);
	OBB playerOBB(playerTrans->pos, ColliderRadius::Ship, playerTrans->rot);

	for (auto largeAsteroid = firstLarge;
		largeAsteroid < firstMedium;
		++largeAsteroid)
	{
		Transform* asteroidTrans;
		transformManager.GetPtr(largeAsteroid->entity, &asteroidTrans);

		Circle asteroid(asteroidTrans->pos, ColliderRadius::Large);
		if (CollisionTests::OBBToCircle(playerOBB, asteroid))
		{
			//std::cout << "Player Be Ded." << std::endl;
		}
	}

	for (auto mediumAsteroid = firstMedium;
		mediumAsteroid < firstSmall;
		++mediumAsteroid)
	{
		Transform* asteroidTrans;
		transformManager.GetPtr(mediumAsteroid->entity, &asteroidTrans);

		Circle asteroid(asteroidTrans->pos, ColliderRadius::Medium);
		if (CollisionTests::OBBToCircle(playerOBB, asteroid))
		{
			//std::cout << "Player Be Ded." << std::endl;
		}
	}

	for (auto smallAsteroid = firstSmall;
		smallAsteroid < endOfAsteroids;
		++smallAsteroid)
	{
		Transform* asteroidTrans;
		transformManager.GetPtr(smallAsteroid->entity, &asteroidTrans);

		Circle asteroid(asteroidTrans->pos, ColliderRadius::Small);
		if (CollisionTests::OBBToCircle(playerOBB, asteroid))
		{
			//std::cout << "Player Be Ded." << std::endl;
		}
	}

#pragma endregion

#pragma region LargeVsAll
	for (auto asteroidA = firstLarge;
		asteroidA < firstMedium - 1;
		++asteroidA)
	{
		Transform* asteroidATrans;
		transformManager.GetPtr(asteroidA->entity, &asteroidATrans);

		// Test all Large vs all other Large
		for (auto asteroidB = asteroidA + 1;
			asteroidB < firstMedium;
			++asteroidB)
		{
			constexpr float largeVsLargeSqRadius = (ColliderRadius::Large + ColliderRadius::Large) *
				(ColliderRadius::Large + ColliderRadius::Large);

			Transform* asteroidBTrans;
			transformManager.GetPtr(asteroidB->entity, &asteroidBTrans);

			float timeOfCollision;
			if (CollisionTests::SweptCircleToCircle(
				asteroidATrans->pos, asteroidA->velocity,
				asteroidBTrans->pos, asteroidB->velocity,
				ColliderRadius::Large, largeVsLargeSqRadius, deltaTime, timeOfCollision))
			{
				CollisionListEntry col;
				col.A = asteroidA->entity;
				col.massA = AsteroidMasses[0];

				col.B = asteroidB->entity;
				col.massB = AsteroidMasses[0];

				col.timeOfCollision = timeOfCollision;
				collisionList.push_back(col);
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

			Transform* asteroidBTrans;
			transformManager.GetPtr(asteroidB->entity, &asteroidBTrans);

			float timeOfCollision;
			if (CollisionTests::SweptCircleToCircle(
				asteroidATrans->pos, asteroidA->velocity,
				asteroidBTrans->pos, asteroidB->velocity,
				ColliderRadius::Large, largeVsMediumSqRadius, deltaTime, timeOfCollision))
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

			Transform* asteroidBTrans;
			transformManager.GetPtr(asteroidB->entity, &asteroidBTrans);

			float timeOfCollision;
			if (CollisionTests::SweptCircleToCircle(
				asteroidATrans->pos, asteroidA->velocity,
				asteroidBTrans->pos, asteroidB->velocity, 
				ColliderRadius::Large, largeVsSmallSqRadius, deltaTime, timeOfCollision))
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
		Transform* asteroidATrans;
		transformManager.GetPtr(asteroidA->entity, &asteroidATrans);

		// Test all Medium vs all other Medium
		for (auto asteroidB = asteroidA + 1;
			asteroidB < firstSmall;
			++asteroidB)
		{
			constexpr float  mediumVsMediumSqRadius = (ColliderRadius::Medium + ColliderRadius::Medium) *
				(ColliderRadius::Medium + ColliderRadius::Medium);

			Transform* asteroidBTrans;
			transformManager.GetPtr(asteroidB->entity, &asteroidBTrans);

			float timeOfCollision;
			if (CollisionTests::SweptCircleToCircle(
				asteroidATrans->pos, asteroidA->velocity,
				asteroidBTrans->pos, asteroidB->velocity,
				ColliderRadius::Medium, mediumVsMediumSqRadius, deltaTime, timeOfCollision))
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

			Transform* asteroidBTrans;
			transformManager.GetPtr(asteroidB->entity, &asteroidBTrans);

			float timeOfCollision;
			if (CollisionTests::SweptCircleToCircle(
				asteroidATrans->pos, asteroidA->velocity,
				asteroidBTrans->pos, asteroidB->velocity,
				ColliderRadius::Medium, mediumVsSmallSqRadius, deltaTime, timeOfCollision))
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
		Transform* asteroidATrans;
		transformManager.GetPtr(asteroidA->entity, &asteroidATrans);

		// Test all Small vs all other Small
		for (auto asteroidB = asteroidA + 1;
			asteroidB < endOfAsteroids;
			++asteroidB)
		{
			constexpr float smallVsSmallSqRadius = (ColliderRadius::Small + ColliderRadius::Small) *
				(ColliderRadius::Small + ColliderRadius::Small);

			Transform* asteroidBTrans;
			transformManager.GetPtr(asteroidB->entity, &asteroidBTrans);

			float timeOfCollision;
			if (CollisionTests::SweptCircleToCircle(
				asteroidATrans->pos, asteroidA->velocity,
				asteroidBTrans->pos, asteroidB->velocity,
				ColliderRadius::Small, smallVsSmallSqRadius, deltaTime, timeOfCollision))
			{
				//std::cout << "Collision between " << asteroidA->id.ToString() << " and " << asteroidB->id.ToString() << "!\n";
			}
		}
	}
#pragma endregion
}


void Physics::ResolveUpdatedMovement(const float deltaTime)
{
	// Start with the earliest collision and resolve the new motion and add the entities that collided to a "dirty set".
	for (auto& collision : collisionList)
	{
		// God I really want std::set.contains()...
		// "If dirtyList doesn't contain either A or B..
		if (dirtyList.find(collision.A) == dirtyList.end() &&
			dirtyList.find(collision.B) == dirtyList.end())
		{
			// Continue resolving collisions, but skip any that involve objects in the dirty set.
			ResolveMove(deltaTime, collision);

			dirtyList.insert(collision.A);
			dirtyList.insert(collision.B);
		}
	}
}


void Physics::ResolveMove(const float deltaTime, CollisionListEntry collision)
{
	// This is probably all kinds of wrong, but I have never studied physics
	// so I'm just sort of making this up as I go along, helped with some
	// frankly crappy youtube videos.

	// https://www.youtube.com/watch?v=Dww4ArU5JF8

	Transform* transA;
	Rigidbody* rigidA;
	transformManager.GetPtr(collision.A, &transA);
	rigidbodyManager.GetPtr(collision.A, &rigidA);

	Transform* transB;
	Rigidbody* rigidB;
	transformManager.GetPtr(collision.B, &transB);
	rigidbodyManager.GetPtr(collision.B, &rigidB);

	Vector2 startPosA = transA->pos + rigidA->velocity * (collision.timeOfCollision * deltaTime);
	Vector2 startPosB = transB->pos + rigidB->velocity * (collision.timeOfCollision * deltaTime);

	Vector2 relPos = startPosB - startPosA;
	Vector2 relVel = rigidB->velocity - rigidA->velocity;

	// Split the problem into two parts: the component normal to the collision
	// and the component tangential to the collision.

	Vector2 impactNormal = relPos.normalized();
	Vector2 impactTangent = impactNormal.Rot90CW();

	// Conservation of Momentum (tangential)
	// Dot(impactTangent, A.velocity) = Dot(impactTangent, endVelA);
	// Dot(impactTangent, B.velocity) = Dot(impactTangent, endVelB);
	float finalATangent = Dot(rigidA->velocity, impactTangent); // A.vel * cos(theta)
	float finalBTangent = Dot(rigidB->velocity, impactTangent); // B.vel * cos(theta)

	// @NOTE: I am most likely mis-using this term here.
	const float e = 0.95f; // Coefficient of restitution
	// e = B.Velocity - A.Velocity / endVelB - endVelA

	// Conservation of Momentum (normal)
	float normalA = Dot(rigidA->velocity, impactNormal); // A.vel * sin(theta)
	float normalB = Dot(rigidB->velocity, impactNormal); // B.vel * sin(theta)
	// massA * normalA + massB * normalB = massA * finalANormal + massB * finalBNormal;

	// Did the algebra and this is what fell out.

	float finalBNormal =
		normalA * collision.massA * e -
		normalB * collision.massA * e +
		normalA * collision.massA + normalB * collision.massB;

	finalBNormal /= collision.massA + collision.massB;

	float finalANormal = e * normalB - e * normalA + finalBNormal;

	// Enqueue the resolved entry.

	ResolvedListEntry resolvedA;
	resolvedA.angularVelocity = rigidA->angularVelocity;
	resolvedA.entity = collision.A;
	resolvedA.rotation = transA->rot;
	resolvedA.position = transA->pos + (rigidA->velocity * collision.timeOfCollision * deltaTime);
	resolvedA.velocity = (impactNormal * finalANormal) + (impactTangent * finalATangent);
	resolvedList.insert(std::make_pair(collision.timeOfCollision, resolvedA));


	ResolvedListEntry resolvedB;
	resolvedB.angularVelocity = rigidB->angularVelocity;
	resolvedB.entity = collision.B;
	resolvedB.rotation = transB->rot;
	resolvedB.position = transB->pos + (rigidB->velocity * collision.timeOfCollision * deltaTime);
	resolvedB.velocity = (impactNormal * finalBNormal) + (impactTangent * finalBTangent);
	resolvedList.insert(std::make_pair(collision.timeOfCollision, resolvedB));
}


void Physics::FinalizeMoves(const float deltaTime)
{
	// Step 9. Iterate MoveList and complete every move.
	for (auto& entry : moveList)
	{
		Transform* trans;
		transformManager.GetPtr(entry.entity, &trans);
		trans->pos.x =
			Math::Repeat(trans->pos.x + (entry.velocity.x * deltaTime), screenAABB.right);
		trans->pos.y =
			Math::Repeat(trans->pos.y + (entry.velocity.y * deltaTime), screenAABB.bottom);

		trans->rot = Math::Repeat(trans->rot + (entry.angularVelocity * deltaTime), 360.0f);
	}

	// Step 10 Iterate ResolvedList and stomp over with revised moves that are legal.
	for (auto& entry : resolvedList)
	{
		ResolvedListEntry resolvedEntry = entry.second;

		Transform* trans;
		transformManager.GetPtr(resolvedEntry.entity, &trans);

		Vector2 finalPos = resolvedEntry.position + (resolvedEntry.velocity * ((1.0f - entry.first) * deltaTime));

		trans->pos.x = Math::Repeat(finalPos.x, screenAABB.right);
		trans->pos.y = Math::Repeat(finalPos.y, screenAABB.bottom);

		trans->rot = Math::Repeat(resolvedEntry.rotation, 360.0f);

		Rigidbody* rigid;
		rigidbodyManager.GetPtr(entry.second.entity, &rigid);
		rigid->velocity = resolvedEntry.velocity;
		rigid->angularVelocity = resolvedEntry.angularVelocity;
	}
}