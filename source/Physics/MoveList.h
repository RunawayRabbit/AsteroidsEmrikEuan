#pragma once

#include <vector>

#include "..\Math\Vector2.h"

#include "..\ECS\Entity.h"
#include "..\ECS\Transform.h"

class MoveList
{
public:
	MoveList();
	void Clear();

	size_t Size() const;
	void Enqueue(Entity entity, Vector2 velocity, float angularVelocity);

private:
	struct Entry
	{
		Entity id;
		Vector2 Velocity;
		float angularVelocity;
		Transform transform; // initialize this by copying from the entityID in the xform manager
	};

	std::vector<Entry> data;
};