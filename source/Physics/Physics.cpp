
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
		moveList.push_back(*rb);
		++colliderCounts[(int)rb->colliderType];
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

	for (auto& count : colliderCounts)
		count = 0;
}

void Physics::DetectInitialCollisions(const float& deltaTime)
{
	// Sort to get our movelist in order.
	std::sort(moveList.begin(), moveList.end());

	// The order of these has to match the order of enum ColliderType defined in ColliderType.h.
	ColliderRanges ranges;
	ranges.shipBegin = moveList.begin();
	ranges.shipEnd = ranges.bulletBegin = ranges.shipBegin + colliderCounts[(int)ColliderType::SHIP];
	ranges.bulletEnd = ranges.largeBegin = ranges.bulletBegin + colliderCounts[(int)ColliderType::BULLET];
	ranges.largeEnd = ranges.mediumBegin = ranges.largeBegin + colliderCounts[(int)ColliderType::LARGE_ASTEROID];
	ranges.mediumEnd = ranges.smallBegin = ranges.mediumBegin + colliderCounts[(int)ColliderType::MEDIUM_ASTEROID];
	ranges.smallEnd = moveList.end();

	ShipVsAsteroid(ranges);
	BulletVsAsteroid(ranges, deltaTime);
	AsteroidVsAsteroid(ranges, deltaTime);
}

void Physics::ShipVsAsteroid(const ColliderRanges& ranges)
{
	for (auto ship = ranges.shipBegin; ship != ranges.shipEnd; ++ship)
	{
		Transform shipTrans;
		if (!transformManager.Get(ship->entity, shipTrans))
		{
			// @TODO: Are you ever going to write that logging module? Because this should be logged.
			continue;
		}
		OBB playerOBB(shipTrans.pos, ColliderRadius::Ship, shipTrans.rot);

		OBBVsSpecificAsteroid(playerOBB, ranges.largeBegin, ranges.largeEnd, ColliderRadius::Large);
		OBBVsSpecificAsteroid(playerOBB, ranges.mediumBegin, ranges.mediumEnd, ColliderRadius::Medium);
		OBBVsSpecificAsteroid(playerOBB, ranges.smallBegin, ranges.smallEnd, ColliderRadius::Small);
	}
}

void Physics::OBBVsSpecificAsteroid(const OBB& ship, std::vector<Rigidbody>::iterator asteroidBegin,
	std::vector<Rigidbody>::iterator asteroidEnd, const float& asteroidRadius)
{
	for (auto asteroid = asteroidBegin; asteroid != asteroidEnd; ++asteroid)
	{
		Transform asteroidTrans;
		transformManager.Get(asteroid->entity, asteroidTrans);

		Circle collider(asteroidTrans.pos, ColliderRadius::Small);
		if (CollisionTests::OBBToCircle(ship, collider))
		{
			std::cout << "Player Be Ded." << std::endl;
		}
	}
}


void Physics::BulletVsAsteroid(const ColliderRanges& ranges, const float& deltaTime)
{
	constexpr float bulletMass = 0; // Fuck it, circles don't have mass. I have decided this.

	for (auto bullet = ranges.bulletBegin; bullet != ranges.bulletEnd; ++bullet)
	{
		constexpr float bulletVsLargeRadius =
			(ColliderRadius::Bullet + ColliderRadius::Large) *
			(ColliderRadius::Bullet + ColliderRadius::Large);
		CircleVsCircles(*bullet, ColliderRadius::Bullet, bulletMass,
			ranges.largeBegin, ranges.largeEnd, AsteroidMasses[0], bulletVsLargeRadius, deltaTime);

		constexpr float bulletVsMediumRadius =
			(ColliderRadius::Bullet + ColliderRadius::Medium) *
			(ColliderRadius::Bullet + ColliderRadius::Medium);
		CircleVsCircles(*bullet, ColliderRadius::Bullet, bulletMass,
			ranges.mediumBegin, ranges.mediumEnd, AsteroidMasses[1], bulletVsMediumRadius, deltaTime);

		constexpr float bulletVsSmallRadius =
			(ColliderRadius::Bullet + ColliderRadius::Small) *
			(ColliderRadius::Bullet + ColliderRadius::Small);
		CircleVsCircles(*bullet, ColliderRadius::Bullet, bulletMass,
			ranges.smallBegin, ranges.smallEnd, AsteroidMasses[2], bulletVsSmallRadius, deltaTime);
	}
}

void Physics::AsteroidVsAsteroid(const ColliderRanges& ranges, const float& deltaTime)
{

	for (auto large = ranges.largeBegin; large != ranges.largeEnd; ++large)
	{
		// Large Vs Large
		constexpr float largeVsLargeSqRadius =
			(ColliderRadius::Large + ColliderRadius::Large) *
			(ColliderRadius::Large + ColliderRadius::Large);

		// @NOTE: starting the range at large+1 guarantees that we don't check A against itself,
		// and that we don't repeat test pairs that have already been computed.
		CircleVsCircles(*large, ColliderRadius::Large, AsteroidMasses[0],
			large + 1, ranges.largeEnd, AsteroidMasses[0], largeVsLargeSqRadius, deltaTime);


		// Large Vs Medium
		constexpr float largeVsMediumSqRadius =
			(ColliderRadius::Large + ColliderRadius::Medium) *
			(ColliderRadius::Large + ColliderRadius::Medium);

		CircleVsCircles(*large, ColliderRadius::Large, AsteroidMasses[0],
			ranges.mediumBegin, ranges.mediumEnd, AsteroidMasses[1], largeVsMediumSqRadius, deltaTime);


		// Large Vs Small
		constexpr float largeVsSmallSqRadius =
			(ColliderRadius::Large + ColliderRadius::Small) *
			(ColliderRadius::Large + ColliderRadius::Small);

		CircleVsCircles(*large, ColliderRadius::Large, AsteroidMasses[0],
			ranges.smallBegin, ranges.smallEnd, AsteroidMasses[2], largeVsSmallSqRadius, deltaTime);
	}


	for (auto medium = ranges.mediumBegin; medium != ranges.mediumEnd; ++medium)
	{

		// Medium Vs Medium
		constexpr float mediumVsMediumSqRadius =
			(ColliderRadius::Medium + ColliderRadius::Medium) *
			(ColliderRadius::Medium + ColliderRadius::Medium);

		// @NOTE: starting the range at medium+1 guarantees that we don't check A against itself,
		// and that we don't repeat test pairs that have already been computed.
		CircleVsCircles(*medium, ColliderRadius::Medium, AsteroidMasses[1],
			medium + 1, ranges.mediumEnd, AsteroidMasses[1], mediumVsMediumSqRadius, deltaTime);


		// Medium Vs Small
		constexpr float mediumVsSmallSqRadius =
			(ColliderRadius::Medium + ColliderRadius::Small) *
			(ColliderRadius::Medium + ColliderRadius::Small);

		CircleVsCircles(*medium, ColliderRadius::Medium, AsteroidMasses[1],
			ranges.smallBegin, ranges.smallEnd, AsteroidMasses[2], mediumVsSmallSqRadius, deltaTime);
	}


	for (auto small = ranges.smallBegin; small != ranges.smallEnd; ++small)
	{
		// Small Vs Small
		constexpr float smallVsSmallSqRadius =
			(ColliderRadius::Small + ColliderRadius::Small) *
			(ColliderRadius::Small + ColliderRadius::Small);

		// @NOTE: starting the range at small+1 guarantees that we don't check A against itself,
		// and that we don't repeat test pairs that have already been computed.
		CircleVsCircles(*small, ColliderRadius::Small, AsteroidMasses[2],
			small + 1, ranges.smallEnd, AsteroidMasses[2], smallVsSmallSqRadius, deltaTime);
	}
}


void Physics::CircleVsCircles(const Rigidbody& circle, const float& circleRadius, const float& circleMass,
	std::vector<Rigidbody>::iterator circlesBegin,	std::vector<Rigidbody>::iterator circlesEnd,
	const float& circlesMass, const float& radiusPlusRadiusSquared, const float& deltaTime)
{
	// Get transform information for our first circle
	Transform circleATrans;
	transformManager.Get(circle.entity, circleATrans);

	// Check our first circle against every circle in the range that we were passed
	for (auto circleB = circlesBegin; circleB != circlesEnd; ++circleB)
	{
		Transform circleBTrans;
		transformManager.Get(circleB->entity, circleBTrans);

		float timeOfCollision;
		if (CollisionTests::SweptCircleToCircle(
			circleATrans.pos, circle.velocity,
			circleBTrans.pos, circleB->velocity,
			circleRadius, radiusPlusRadiusSquared, deltaTime, timeOfCollision))
		{
			CollisionListEntry col;
			col.A = circle.entity;
			col.massA = circleMass;

			col.B = circleB->entity;
			col.massB = circlesMass;

			col.timeOfCollision = timeOfCollision;
			collisionList.push_back(col);
		}		
	}
}

void Physics::ResolveUpdatedMovement(const float& deltaTime)
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


void Physics::ResolveMove(const float& deltaTime, CollisionListEntry collision)
{
	// This is probably all kinds of wrong, but I have never studied physics
	// so I'm just sort of making this up as I go along, helped with some
	// frankly crappy youtube videos.

	// https://www.youtube.com/watch?v=Dww4ArU5JF8

	Transform* transA;
	Rigidbody* rigidA;
	transformManager.GetMutable(collision.A, transA);
	rigidbodyManager.GetPtr(collision.A, &rigidA);

	Transform* transB;
	Rigidbody* rigidB;
	transformManager.GetMutable(collision.B, transB);
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
	resolvedA.position = transA->pos + (rigidA->velocity * collision.timeOfCollision * deltaTime);
	resolvedA.velocity = (impactNormal * finalANormal) + (impactTangent * finalATangent);
	resolvedList.insert(std::make_pair(collision.timeOfCollision, resolvedA));


	ResolvedListEntry resolvedB;
	resolvedB.angularVelocity = rigidB->angularVelocity;
	resolvedB.entity = collision.B;
	resolvedB.position = transB->pos + (rigidB->velocity * collision.timeOfCollision * deltaTime);
	resolvedB.velocity = (impactNormal * finalBNormal) + (impactTangent * finalBTangent);
	resolvedList.insert(std::make_pair(collision.timeOfCollision, resolvedB));
}


void Physics::FinalizeMoves(const float& deltaTime)
{
	// Step 9. Iterate MoveList and complete every move.
	for (auto& entry : moveList)
	{
		Transform* trans;
		transformManager.GetMutable(entry.entity, trans);
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
		transformManager.GetMutable(resolvedEntry.entity, trans);

		Vector2 finalPos = resolvedEntry.position + (resolvedEntry.velocity * ((1.0f - entry.first) * deltaTime));

		trans->pos.x = Math::Repeat(finalPos.x, screenAABB.right);
		trans->pos.y = Math::Repeat(finalPos.y, screenAABB.bottom);

		Rigidbody* rigid;
		rigidbodyManager.GetPtr(entry.second.entity, &rigid);
		rigid->velocity = resolvedEntry.velocity;
		rigid->angularVelocity = resolvedEntry.angularVelocity;
	}
}