#pragma once


#include <unordered_set>
#include <string>
#include <iostream>

#include "Entity.h"

class EntityManager
{
public:
	EntityManager();

	Entity Create();
	bool Exists(Entity entity) const;
	void Destroy(Entity entity);

	void DEBUGPrintAllEntities()
	{
		for (Entity entity : entities)
		{
			std::cout << entity.id << " ";
		}
	}

private:
	Entity nextEntity;
	std::unordered_set<Entity> entities;
};