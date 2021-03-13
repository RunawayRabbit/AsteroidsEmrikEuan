
#include <unordered_set>
#include <algorithm> // for min and max

#include "Physics.h"
#include "ColliderType.h"
#include "CollisionTests.h"

#include "..\ECS\RigidbodyManager.h"
#include "..\ECS\TransformManager.h"

#include "..\Math\Math.h"
#include "..\Math\OBB.h"
#include "..\Math\Vector2.h"

Physics::Physics(TransformManager& transformManager, RigidbodyManager& rigidbodyManager, const AABB& screenAABB) :
	transformManager(transformManager),
	rigidbodyManager(rigidbodyManager),
	screenAABB(screenAABB),
	chunkSizeX(screenAABB.max.x / chunksX),
	chunkSizeY(screenAABB.max.x / chunksY)
{
}

void Physics::Enqueue(const Rigidbody& rb, const float& deltaTime)
{
	// Get an AABB for the rigidbody using it's transform
	Transform rbTrans;
	transformManager.Get(rb.entity, rbTrans);
	AABB rbAABB = ColliderRadius::GetAABB(rb.colliderType, rbTrans.pos);

	// Pad the AABB by the velocity.
	Vector2 deltaPosition = rb.velocity * deltaTime;
	rbAABB.min.x = std::min(rbAABB.min.x, rbAABB.min.x + deltaPosition.x);
	rbAABB.min.y = std::min(rbAABB.min.y, rbAABB.min.y + deltaPosition.y);
	rbAABB.max.x = std::max(rbAABB.max.x, rbAABB.max.x + deltaPosition.x);
	rbAABB.max.y = std::max(rbAABB.max.y, rbAABB.max.y + deltaPosition.y);

	Vector2 accumulator = Vector2::zero();
	int startTileX = -1;
	int startTileY = -1;
	while (accumulator.x < rbAABB.min.x)
	{
		accumulator.x += chunkSizeX;
		++startTileX;
	}
	while (accumulator.y < rbAABB.min.y)
	{
		accumulator.y += chunkSizeY;
		++startTileY;
	}
	// @NOTE: Logic here is a bit tricky. At this point, startTile indicates the
	//  "lowest" tile that the AABB overlaps, and accumulator is sitting in the
	//  maximum corner of that tile.

	// Using the same method, we advance the max accumulator to find the maximum.
	int endTileX = startTileX;
	int endTileY = startTileY;
	while (accumulator.x < rbAABB.max.x)
	{
		accumulator.x += chunkSizeX;
		++endTileX;
	}
	while (accumulator.y < rbAABB.max.y)
	{
		accumulator.y += chunkSizeY;
		++endTileY;
	}


	// Enqueue the rigidbody into the appropriate moveLists by the chunk it's in.
	auto mod = [](int a, int b) { return (b + (a % b)) % b; };
	for (auto y = startTileY; y <= endTileY; ++y)
	{
		// Wrap our Y coordinate if we have to.
		float wrappedY = rbTrans.pos.y;
		if (y < 0)
			wrappedY += screenAABB.max.y;
		else if (y >= chunksY)
			wrappedY -= screenAABB.max.y;

		for (auto x = startTileX; x <= endTileX; ++x)
		{
			// Wrap the element in X if we have to.
			float wrappedX = rbTrans.pos.x;
			if (x < 0)
				wrappedX += screenAABB.max.x;
			else if (x >= chunksX)
				wrappedX -= screenAABB.max.x;

			// Calculate the chunk index and enqueue
			int chunkIndex = mod(y, chunksY) * chunksX + mod(x, chunksX);
			moveLists[chunkIndex].Enqueue({ rb, Vector2(wrappedX, wrappedY) });
		}
	}
}

void Physics::Simulate(const float& deltaTime)
{
	// Start our worker threads churning through the Initial collision tests.
	for(auto i = 0; i < chunkCount; ++i)
	{
		workers[i] = std::async([this, i, deltaTime]() -> std::vector<Physics::CollisionListEntry>
			{
				return DetectInitialCollisions(moveLists[i], deltaTime);
			});
	}
	for (auto& worker : workers)
	{
		auto result = worker.get();
		collisionList.reserve(collisionList.size() + result.size());
		std::copy(result.begin(), result.end(), std::back_inserter(collisionList));
	}

	int solverIterations = 0;
	int resolvedCount = 0;
	do
	{
		// Step 4. Start with the earliest collision and resolve the new motion and add the entities that collided to a "dirty set".
		// Step 5. Continue resolving collisions, but skip any that involve objects in the dirty set.
		std::sort(collisionList.begin(), collisionList.end());
		auto resolvedThisIteration = ResolveUpdatedMovement(deltaTime);

		// Step 6. Clear the collision list and the dirty set.
		collisionList.clear();
		dirtyList.clear();

		// Step 7. Check the NEW elements in resolved list against the full move list, looking for further collisions.
		DetectSecondaryCollisions(resolvedThisIteration);

		// Step 7.5. Integrate the new resolved entries into the list.
		resolvedList.reserve(resolvedList.size() + resolvedThisIteration.size());
		resolvedList.insert(resolvedList.end(), resolvedThisIteration.begin(), resolvedThisIteration.end());

		// Step 8. If the collision list is empty, continue. Otherwise, loop..
		++solverIterations;
	} while (collisionList.size() > 0 && solverIterations < MaxSolverIterations);

	std::sort(resolvedList.begin(), resolvedList.end(),
		[](const ResolvedListEntry& a, ResolvedListEntry& b) ->bool
		{
			return a.time < b.time;
		});

	FinalizeMoves(deltaTime);
	End();
}

void Physics::End()
{
	for (auto& moveList : moveLists)
		moveList.Clear();

	collisionList.clear();
	resolvedList.clear();
	dirtyList.clear();
}

std::vector<Physics::CollisionListEntry> Physics::DetectInitialCollisions(MoveList& moveList, const float& deltaTime)
{
	std::vector<CollisionListEntry> collisions;
	if (moveList.Size() == 0) return collisions;

	// Sort to get our movelist in order.
	std::sort(moveList.begin(), moveList.end(), [](const MoveList::Entry& a, const MoveList::Entry& b) -> bool
		{
			return a.rb.colliderType < b.rb.colliderType;
		});

	// The order of these has to match the order of enum ColliderType defined in ColliderType.h.
	MoveList::ColliderRanges ranges;
	ranges.shipBegin = moveList.begin();
	ranges.shipEnd = ranges.bulletBegin = ranges.shipBegin + moveList.GetColliderCount(ColliderType::SHIP);
	ranges.bulletEnd = ranges.largeBegin = ranges.bulletBegin + moveList.GetColliderCount(ColliderType::BULLET);
	ranges.largeEnd = ranges.mediumBegin = ranges.largeBegin + moveList.GetColliderCount(ColliderType::LARGE_ASTEROID);
	ranges.mediumEnd = ranges.smallBegin = ranges.mediumBegin + moveList.GetColliderCount(ColliderType::MEDIUM_ASTEROID);
	ranges.smallEnd = moveList.end();

	ShipVsAsteroid(ranges, collisions);
	BulletVsAsteroid(ranges, collisions, deltaTime);
	AsteroidVsAsteroid(ranges, collisions, deltaTime);

	return collisions;
}



void Physics::ShipVsAsteroid(const MoveList::ColliderRanges& ranges, std::vector<CollisionListEntry>& collisions)
{
	for (auto ship = ranges.shipBegin; ship != ranges.shipEnd; ++ship)
	{
		Transform shipTrans;
		if (!transformManager.Get(ship->rb.entity, shipTrans))
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

void Physics::OBBVsSpecificAsteroid(const OBB& ship, std::vector<MoveList::Entry>::iterator asteroidBegin,
	std::vector<MoveList::Entry>::iterator asteroidEnd, const float& asteroidRadius)
{
	for (auto asteroid = asteroidBegin; asteroid != asteroidEnd; ++asteroid)
	{
		Circle collider(asteroid->pos, asteroidRadius);
		if (CollisionTests::OBBToCircle(ship, collider))
		{
			std::cout << "Player Be Ded." << std::endl;
		}
	}
}


void Physics::BulletVsAsteroid(const MoveList::ColliderRanges& ranges, std::vector<CollisionListEntry> &collisions, const float& deltaTime)
{
	constexpr float bulletMass = 0; // Fuck it, circles don't have mass. I have decided this.

	for (auto bullet = ranges.bulletBegin; bullet != ranges.bulletEnd; ++bullet)
	{
		constexpr float bulletVsLargeRadius =
			(ColliderRadius::Bullet + ColliderRadius::Large) *
			(ColliderRadius::Bullet + ColliderRadius::Large);
		CircleVsCircles(*bullet, ColliderRadius::Bullet, bulletMass,
			ranges.largeBegin, ranges.largeEnd, AsteroidMasses[0], bulletVsLargeRadius, deltaTime, collisions);

		constexpr float bulletVsMediumRadius =
			(ColliderRadius::Bullet + ColliderRadius::Medium) *
			(ColliderRadius::Bullet + ColliderRadius::Medium);
		CircleVsCircles(*bullet, ColliderRadius::Bullet, bulletMass,
			ranges.mediumBegin, ranges.mediumEnd, AsteroidMasses[1], bulletVsMediumRadius, deltaTime, collisions);

		constexpr float bulletVsSmallRadius =
			(ColliderRadius::Bullet + ColliderRadius::Small) *
			(ColliderRadius::Bullet + ColliderRadius::Small);
		CircleVsCircles(*bullet, ColliderRadius::Bullet, bulletMass,
			ranges.smallBegin, ranges.smallEnd, AsteroidMasses[2], bulletVsSmallRadius, deltaTime, collisions);
	}
}

void Physics::AsteroidVsAsteroid(const MoveList::ColliderRanges& ranges, std::vector<CollisionListEntry>& collisions, const float& deltaTime)
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
			large + 1, ranges.largeEnd, AsteroidMasses[0], largeVsLargeSqRadius, deltaTime, collisions);


		// Large Vs Medium
		constexpr float largeVsMediumSqRadius =
			(ColliderRadius::Large + ColliderRadius::Medium) *
			(ColliderRadius::Large + ColliderRadius::Medium);

		CircleVsCircles(*large, ColliderRadius::Large, AsteroidMasses[0],
			ranges.mediumBegin, ranges.mediumEnd, AsteroidMasses[1], largeVsMediumSqRadius, deltaTime, collisions);


		// Large Vs Small
		constexpr float largeVsSmallSqRadius =
			(ColliderRadius::Large + ColliderRadius::Small) *
			(ColliderRadius::Large + ColliderRadius::Small);

		CircleVsCircles(*large, ColliderRadius::Large, AsteroidMasses[0],
			ranges.smallBegin, ranges.smallEnd, AsteroidMasses[2], largeVsSmallSqRadius, deltaTime, collisions);
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
			medium + 1, ranges.mediumEnd, AsteroidMasses[1], mediumVsMediumSqRadius, deltaTime, collisions);


		// Medium Vs Small
		constexpr float mediumVsSmallSqRadius =
			(ColliderRadius::Medium + ColliderRadius::Small) *
			(ColliderRadius::Medium + ColliderRadius::Small);

		CircleVsCircles(*medium, ColliderRadius::Medium, AsteroidMasses[1],
			ranges.smallBegin, ranges.smallEnd, AsteroidMasses[2], mediumVsSmallSqRadius, deltaTime, collisions);
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
			small + 1, ranges.smallEnd, AsteroidMasses[2], smallVsSmallSqRadius, deltaTime, collisions);
	}
}


void Physics::CircleVsCircles(const MoveList::Entry& circle, const float& circleRadius, const float& circleMass,
	std::vector<MoveList::Entry>::iterator circlesBegin,	std::vector<MoveList::Entry>::iterator circlesEnd,
	const float& circlesMass, const float& radiusPlusRadiusSquared, const float& deltaTime, std::vector<CollisionListEntry>& collisions)
{
	// Check our first circle against every circle in the range that we were passed
	for (auto circleB = circlesBegin; circleB != circlesEnd; ++circleB)
	{
		if (circle.rb.entity == circleB->rb.entity) continue;

		float timeOfCollision;
		if (CollisionTests::SweptCircleToCircle(
			circle.pos, circle.rb.velocity,
			circleB->pos, circleB->rb.velocity,
			circleRadius, radiusPlusRadiusSquared, deltaTime, timeOfCollision))
		{
			CollisionListEntry col;
			col.A = circle.rb.entity;
			col.massA = circleMass;

			col.B = circleB->rb.entity;
			col.massB = circlesMass;

			col.timeOfCollision = timeOfCollision;
			collisions.push_back(col);
		}		
	}
}

std::vector<Physics::ResolvedListEntry> Physics::ResolveUpdatedMovement(const float& deltaTime)
{
	std::vector<ResolvedListEntry> resolved;

	// Start with the earliest collision and resolve the new motion and add the entities that collided to a "dirty set".
	for (auto& collision : collisionList)
	{
		// God I really want std::set.contains()...
		// "If dirtyList doesn't contain either A or B..
		if (dirtyList.find(collision.A) == dirtyList.end() &&
			dirtyList.find(collision.B) == dirtyList.end())
		{
			// Continue resolving collisions, but skip any that involve objects in the dirty set.
			auto moves = ResolveMove(deltaTime, collision);
			resolved.push_back(moves[0]);
			resolved.push_back(moves[1]);

			dirtyList.insert(collision.A);
			dirtyList.insert(collision.B);
		}
	}

	return resolved;
}


std::array<Physics::ResolvedListEntry, 2> Physics::ResolveMove(const float& deltaTime, CollisionListEntry collision)
{
	// This is probably all kinds of wrong, but I have never studied physics
	// so I'm just sort of making this up as I go along, helped with some
	// frankly crappy youtube videos.

	// https://www.youtube.com/watch?v=Dww4ArU5JF8

	Transform* transA;
	Rigidbody* rigidA;
	transformManager.GetMutable(collision.A, transA);
	rigidbodyManager.GetMutable(collision.A, rigidA);

	Transform* transB;
	Rigidbody* rigidB;
	transformManager.GetMutable(collision.B, transB);
	rigidbodyManager.GetMutable(collision.B, rigidB);

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
	resolvedA.time = collision.timeOfCollision;

	ResolvedListEntry resolvedB;
	resolvedB.angularVelocity = rigidB->angularVelocity;
	resolvedB.entity = collision.B;
	resolvedB.position = transB->pos + (rigidB->velocity * collision.timeOfCollision * deltaTime);
	resolvedB.velocity = (impactNormal * finalBNormal) + (impactTangent * finalBTangent);
	resolvedB.time = collision.timeOfCollision;

	return { resolvedA, resolvedB };
}



void Physics::FinalizeMoves(const float& deltaTime)
{
	// Step 9, create a new MoveList set that contains no duplicates.
	std::unordered_set<MoveList::Entry> UniqueMoves;
	for(auto& moveList : moveLists)
		for (auto& move: moveList)
			UniqueMoves.insert(move);


	// Step 9.5.. Iterate MoveList and complete every move.
	for (auto& entry : UniqueMoves)
	{
		Transform* trans;
		transformManager.GetMutable(entry.rb.entity, trans);
		trans->pos.x =
			Math::Repeat(trans->pos.x + (entry.rb.velocity.x * deltaTime), screenAABB.right);
		trans->pos.y =
			Math::Repeat(trans->pos.y + (entry.rb.velocity.y * deltaTime), screenAABB.bottom);

		trans->rot = Math::Repeat(trans->rot + (entry.rb.angularVelocity * deltaTime), 360.0f);
	}

	// Step 10 Iterate ResolvedList and stomp over with revised moves that are legal.
	for (auto& entry : resolvedList)
	{
		ResolvedListEntry resolvedEntry = entry;

		Transform* trans;
		transformManager.GetMutable(resolvedEntry.entity, trans);

		Vector2 finalPos = resolvedEntry.position + (resolvedEntry.velocity * ((1.0f - entry.time) * deltaTime));

		trans->pos.x = Math::Repeat(finalPos.x, screenAABB.right);
		trans->pos.y = Math::Repeat(finalPos.y, screenAABB.bottom);

		Rigidbody* rigid;
		rigidbodyManager.GetMutable(entry.entity, rigid);
		rigid->velocity = resolvedEntry.velocity;
		rigid->angularVelocity = resolvedEntry.angularVelocity;
	}
}

void Physics::DetectSecondaryCollisions(const std::vector<Physics::ResolvedListEntry> resolvedThisIteration)
{
	// Step 7. Check the NEW elements in resolved list against the full move list, looking for further collisions.
}