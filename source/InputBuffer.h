#pragma once

#include <set>
#include <map>

#include "InputDefinitions.h"

struct InputBuffer
{
	std::set<InputOneShot> oneShots;
	std::set<InputToggle> toggles;

	int mouseX = 0;
	int mouseY = 0;

	inline bool Contains(const InputToggle& toggle) const
	{
		return toggles.find(toggle) != toggles.end();
	}

	inline bool Contains(const InputOneShot& oneShot) const
	{
		return oneShots.find(oneShot) != oneShots.end();
	}
};