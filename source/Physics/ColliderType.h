#pragma once

enum class ColliderType
{
	NONE,
	SHIP,

	LARGE_ASTEROID,
	MEDIUM_ASTEROID,
	SMOL_ASTEROID,
};

namespace ColliderRadius
{
	//@TODO: Fix these numbers
	constexpr static float Ship = 24;

	constexpr static float Large = 63/2;
	constexpr static float Medium = 28.32/2;
	constexpr static float Small = 12/2;
}
