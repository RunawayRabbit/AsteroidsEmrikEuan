#pragma once

#include <iostream>

#include "..\Math\Math.h"
#include "..\Math\Vector2.h"
#include "..\Math\Circle.h"
#include "..\Math\AABB.h"

namespace CollisionTests
{
	// Returns true if we will collide this frame.
	bool SweptCircleToCircle(const Circle& A, const Vector2& velA,
		const Circle& B, const Vector2& velB, const float& deltaTime,
		float* timeUntilCollision)
	{
		const Vector2 startPositionDelta = B.center - A.center;	//vector from A0 to B0
		const float combinedRadius = A.radius + B.radius;
		const float combinedRadiusSq = combinedRadius * combinedRadius;

		if (Dot(startPositionDelta, startPositionDelta) <= combinedRadiusSq)
		{
			// Circles are currently intersecting.
			// @TODO: Not sure what to do in this case, as it shouldn't
			// ever happen?
			std::cout << "ERROR: Called CircleVsCircle on two currently overlapping circles!\n";

			*timeUntilCollision = 0;
			return true;
		}

		//const Vector2 velA = A1 - A.center;	//vector from A0 to A1
		//const Vector2 velB = B1 - B.center;	//vector from B0 to B1
		const Vector2 relativeVelocity = (velB - velA) * deltaTime;	// this is in units per frame.


		const float a = Dot(relativeVelocity, relativeVelocity); // t*t
		const float b = 2 * Dot(relativeVelocity, startPositionDelta); // t
		const float c = Dot(startPositionDelta, startPositionDelta) - combinedRadiusSq; // constant


		float root1, root2;
		if (Math::QuadraticFormula(a, b, c, root1, root2))
		{
			*timeUntilCollision = std::min(root1, root2);
			if (*timeUntilCollision > 0.0f &&
				*timeUntilCollision < 1.0f)
			{
				*timeUntilCollision *= deltaTime;
				return true;
			}
		}
		return false;
	}

	// This test is *very simple* because our specific usecase does not call for
	// a swept test, and I don't want to have to implement full GJK and minkowski stuff.
	bool OBBToCircle(const OBB& OBB, const Circle& circle)
	{
		return OBB.DistanceBetweenSq(circle.center) < circle.radius * circle.radius;
	}	
}