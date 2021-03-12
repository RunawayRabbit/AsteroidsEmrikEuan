#pragma once

enum class ColliderType
{
	SHIP,

	BULLET,

	LARGE_ASTEROID,
	MEDIUM_ASTEROID,
	SMOL_ASTEROID,

	COUNT,
};

namespace ColliderRadius
{
	//@TODO: Fix these numbers
	constexpr static float Ship = 24.0f / 2.0f;

	constexpr static float Bullet = 4.0f / 2.0f;

	constexpr static float Large = 62.31f / 2.0f;
	constexpr static float Medium = 28.32f / 2.0f;
	constexpr static float Small = 8.43f / 2.0f;
}
