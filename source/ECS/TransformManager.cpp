
#include "TransformManager.h"
#include "EntityManager.h"


TransformManager::TransformManager(int capacity)
{
	transforms.reserve(capacity);
}


bool TransformManager::Get(const Entity entity, Transform& transform) const
{
	//@TODO This performs a copy when what I *really* want is an immutable
	// reference to the existing data. Any way to fix that?
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
	// @IMPORTANT
	// @BUG
	// @STUB
	/*

	// @TODO: Looping over every transform is inefficient. How do you do this better?
	for (auto& transform : transforms) // It appears that auto& transform : transforms pulls a nullptr??!?!?!
	{
		if (!entityManager.Exists(transform.first)) // This line causes a horrific crash.
		{
			transforms.erase(transform.first);
		}
	}

	*/
}