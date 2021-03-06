#pragma once

#include <iostream>

#include "..\Math\Math.h"
#include "..\Math\Vector2.h"
#include "..\Math\Circle.h"
#include "..\Math\AABB.h"

namespace CollisionTests
{

	bool SweptCircleToCircle(const Vector2& centerA, const Vector2& velA,
		const Vector2& centerB, const Vector2& velB, const float& combinedRadiiSq,
		const float& deltaTime,	float* timeUntilCollision)
	{
		const Vector2 startPositionDelta = centerB - centerA;

		const float constantTerm = Dot(startPositionDelta, startPositionDelta) - combinedRadiiSq;
		if (constantTerm < 0.0f)
		{
			// Circles are currently intersecting.

			*timeUntilCollision = 0;
			return true;
		}

		const Vector2 relativeVelocity = (velB - velA) * deltaTime;	// this is in units per frame.
		const float squaredTerm = Dot(relativeVelocity, relativeVelocity); // t*t
		if (squaredTerm < 0.0001f)
		{
			// Circles are relatively stationary
			return false;
		}

		const float scalarTerm = 2 * Dot(relativeVelocity, startPositionDelta); // t
		if (scalarTerm >= 0.0f)
		{
			// Circles are moving away from each other, all roots will be negative.
			return false;
		}

		const float determinant = (scalarTerm * scalarTerm) - squaredTerm * constantTerm;
		if (determinant < 0.0f)
		{
			// All roots are complex.
			return false;
		}

		// We have our collision!

		*timeUntilCollision = (-scalarTerm - sqrt(determinant)) / squaredTerm;
		if (*timeUntilCollision < 1.0f)
			return true;
		else
			return false;
	}

	// Adapted from https://www.gamasutra.com/view/feature/131790/simple_intersection_tests_for_games.php?page=2
	// Returns true if we will collide this frame and fills timeUntilCollision with the fraction of deltaTime at
	// which the collision will take place. (t.ex: timeUntilCollision == 0.5f means we will collide in exactly
	// half a frame.)
	bool SweptCircleToCircle(const Circle& A, const Vector2& velA,
		const Circle& B, const Vector2& velB, const float& deltaTime,
		float* timeUntilCollision)
	{
		const float combinedRadius = A.radius + B.radius;
		const float combinedRadiusSq = combinedRadius * combinedRadius;

		return SweptCircleToCircle(A.center, velA, B.center, velB, combinedRadiusSq, deltaTime, timeUntilCollision);
	}



	// This test is *very simple* because our specific usecase does not call for
	// a swept test, and I don't want to have to implement full GJK/minkowski stuff.
	bool OBBToCircle(const OBB& OBB, const Circle& circle)
	{
		return OBB.DistanceBetweenSq(circle.center) < circle.radius * circle.radius;
	}	
}