
#include "EntityManager.h"
#include "..\State\Time.h"

EntityManager::EntityManager(const Time& time) :
	time(time)
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

void EntityManager::DestroyDelayed(Entity entity, const float& seconds)
{
	deathRow.push({ time.Now() + seconds, entity });
}

void EntityManager::GarbageCollect()
{
	while (deathRow.size() > 0)
	{
		auto element = deathRow.top();
		if (element.first < time.Now())
		{
			Destroy(element.second);
			deathRow.pop();
		}
		else
		{
			return;
		}
	}
}