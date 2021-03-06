#pragma once

#include "..\Math\Circle.h"


#include <vector>
#include <iterator>

std::vector<Vector2> asteroidPositions;


#if 0

void shrug(OBB Ship, Iter firstLarge, Iter firstMedium, Iter firstSmall, Iter endOfAsteroids)
{
	// Inputs are Vector2 iterators representing positions of colliders.


#pragma region Ship_vs_All

	for (Iter largeAsteroid = firstLarge;
		largeAsteroid < firstMedium;
		++largeAsteroid)
	{
		// Ship vs all large

	}

	for (Iter mediumAsteroid = firstMedium;
		mediumAsteroid < firstSmall;
		++mediumAsteroid)
	{
		// Ship vs all medium

	}

	for (Iter smallAsteroid = firstSmall;
		smallAsteroid < endOfAsteroids;
		++smallAsteroid)
	{
		// Ship vs all small

	}

#pragma endregion


	// @TODO: Very much simd-able.

#pragma region Large_vs_All
	for (Iter asteroidA = firstLarge;
		asteroidA < firstMedium - 1;
		++asteroidA)
	{
		// Test all Large vs all other Large
		for (Iter asteroidB = firstLarge + 1;
			asteroidB < firstMedium;
			++asteroidB)
		{
			constexpr float  largeVsLargeSqRadius = (largeAsteroidRadius + largeAsteroidRadius) *
				(largeAsteroidRadius + largeAsteroidRadius)
		}

		// Test all Large vs all Medium
		for (Iter asteroidB = firstMedium;
			asteroidB < firstSmall;
			++asteroidB)
		{
			constexpr float  largeVsMediumSqRadius = (largeAsteroidRadius + mediumAsteroidRadius) *
				(largeAsteroidRadius + mediumAsteroidRadius)
		}

		// Test all Large vs all Small
		for (Iter asteroidB = firstSmall;
			asteroidB < endOfAsteroids;
			++asteroidB)
		{
			constexpr float  largeVsSmallSqRadius = (largeAsteroidRadius + smallAsteroidRadius) *
				(largeAsteroidRadius + smallAsteroidRadius)
		}
	}
#pragma endregion

#pragma region Medium_vs_All
	for (Iter asteroidA = firstMedium;
		asteroidA < firstSmall - 1;
		++asteroidA)
	{

		// Test all Medium vs all other Medium
		for (Iter asteroidB = firstMedium + 1;
			asteroidB < firstSmall;
			++asteroidB)
		{
			constexpr float  mediumVsMediumSqRadius = (mediumAsteroidRadius + mediumAsteroidRadius) *
				(mediumAsteroidRadius + mediumAsteroidRadius)
		}

		// Test all Medium vs all Small
		for (Iter asteroidB = firstSmall;
			asteroidB < endOfAsteroids;
			++asteroidB)
		{
			constexpr float  mediumVsSmallSqRadius = (mediumAsteroidRadius + smallAsteroidRadius) *
				(mediumAsteroidRadius + smallAsteroidRadius)
		}
	}
#pragma endregion

#pragma region Small_vs_Small
	for (Iter asteroidA = firstSmall;
		asteroidA < endOfAsteroids - 1;
		++asteroidA)
	{

		// Test all Small vs all other Small
		for (Iter asteroidB = firstSmall + 1;
			asteroidB < endOfAsteroids;
			++asteroidB)
		{
			constexpr float smallVsSmallSqRadius = (smallAsteroidRadius + smallAsteroidRadius) *
				(smallAsteroidRadius + smallAsteroidRadius)
		}
	}
#pragma endregion
}

#endif