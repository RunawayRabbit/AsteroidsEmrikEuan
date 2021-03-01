#pragma once

#include <vector>

#include "Collider.h"

class ColliderManager
{
public:
	const std::vector<Collider> GetColliders() const;

private:
	std::vector<Collider> allColliders;
};