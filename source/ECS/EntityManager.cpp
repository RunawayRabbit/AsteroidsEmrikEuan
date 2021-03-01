
#include "EntityManager.h"

EntityManager::EntityManager()
{
	_nextEntity = Entity();
}

Entity EntityManager::Create()

{
	// Scan for an open slot
	_nextEntity++;
	while (Exists(_nextEntity))
		_nextEntity++;

	// Create and return
	_entities.emplace(_nextEntity);
	return _nextEntity;
}

bool EntityManager::Exists(Entity entity) const
{
	return _entities.count(entity) > 0;
}

void EntityManager::Destroy(Entity entity)
{
	_entities.erase(entity);
}