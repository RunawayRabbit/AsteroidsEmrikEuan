#pragma once

enum class ColliderType
{
	NONE,

	SHIP,

	BULLET,

	LARGE_ASTEROID,
	MEDIUM_ASTEROID,
	SMOL_ASTEROID,
};

namespace ColliderRadius
{
	//@TODO: Fix these numbers
	constexpr static float Ship = 24 / 2;

	constexpr static float Bullet = 4 / 2;

	constexpr static float Large = 62.31 / 2;
	constexpr static float Medium = 28.32 / 2;
	constexpr static float Small = 10.43 / 2;
}
