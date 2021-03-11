
#include "TransformManager.h"
#include "EntityManager.h"


TransformManager::TransformManager(int capacity)
{
	transforms.reserve(capacity);
}


bool TransformManager::Get(const Entity entity, Transform& transform) const
{
	auto search = transforms.find(entity);

	if (search != transforms.end()) {
		transform = search->second;
		return true;
	}
	return false;
}

bool TransformManager::GetMutable(const Entity entity, Transform*& transform)
{
	auto search = transforms.find(entity);

	if (search != transforms.end()) {
		transform = &search->second;
		return true;
	}
	return false;
}


void TransformManager::Add(const Entity entity, const Transform transform)
{
	transforms.insert({ entity, transform });
}

void TransformManager::GarbageCollect(const EntityManager& entityManager)
{
	// @TODO: Looping over every transform is inefficient. How do you do this better?
	for (auto& transform : transforms)
	{
		if (!entityManager.Exists(transform.first))
		{
			transforms.erase(transform.first);
		}
	}
}