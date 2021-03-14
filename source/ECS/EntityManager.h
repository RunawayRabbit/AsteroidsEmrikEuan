#pragma once


#include <unordered_set>
#include <queue>
#include <string>
#include <iostream>

#include "Entity.h"

class Timer;

class EntityManager
{
public:
	EntityManager(const Timer& time);

	Entity Create();
	bool Exists(Entity entity) const;
	void Destroy(Entity entity);
	void DestroyDelayed(Entity entity, const float& seconds);

	void GarbageCollect();

	void DEBUGPrintAllEntities()
	{
		for (Entity entity : entities)
		{
			std::cout << entity.id << " ";
		}
	}

private:
	const Timer& time;

	Entity nextEntity;
	std::unordered_set<Entity> entities;

	std::priority_queue<
		std::pair<float, Entity>,
		std::vector<std::pair<float, Entity>>,
		std::greater<std::pair<float, Entity>>> deathRow;
};