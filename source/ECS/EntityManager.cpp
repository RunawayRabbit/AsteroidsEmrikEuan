
#include "EntityManager.h"

EntityManager::EntityManager()
{
	nextEntity = Entity();
}

Entity EntityManager::Create()

{
	// Scan for an open slot
	nextEntity++;
	while (Exists(nextEntity))
		nextEntity++;

	// Create and return
	entities.emplace(nextEntity);
	return nextEntity;
}

bool EntityManager::Exists(Entity entity) const
{
	return entities.count(entity) > 0;
}

void EntityManager::Destroy(Entity entity)
{
	entities.erase(entity);
}