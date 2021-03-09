
#include "TransformManager.h"
#include "EntityManager.h"


/* @TODO:
	Becoming more and more clear that a lookup table for transforms would
	offer a huge performance benefit for the physics system.
*/

TransformManager::TransformManager(int capacity)
{
	size = 0;
	Allocate(capacity);
}

TransformManager::~TransformManager()
{
	free(buffer);
}

bool TransformManager::Get(const Entity entity, Transform* transform) const
{
	size_t index;
	if (Lookup(entity, &index))
	{
		*transform = *(transforms + index);
		return true;
	}
	return false;
}

bool TransformManager::GetPtr(const Entity entity, Transform** transform)
{
	size_t index;
	if (Lookup(entity, &index))
	{
		*transform = (transforms + index);
		return true;
	}
	return false;
}

bool TransformManager::Set(const Entity entity, const Transform transform)
{
	size_t index;
	if (Lookup(entity, &index))
	{
		*(transforms + index) = transform;
		return true;
	}
	return false;
}

void TransformManager::Add(const Entity entity, const Transform transform)
{
	if (size == capacity)
	{
		// We're about to overrun our buffer, we gotta scale.
		Allocate((size * 2));
	}

#if DEBUG_STUFF


	if (auto index = Lookup(entity))
	{
		// Tried to add multiple transforms to the same entity. We don't allow that!
		_ASSERT(false);
	}

#endif

	// Insert our data at the back of the data store
	*(entities + size) = entity;
	*(transforms + size) = transform;

	++size;
}

void TransformManager::Remove(const Entity entity)
{
	size_t index;
	if (Lookup(entity, &index))
	{
		// swap our entity to the back
		Entity* lastActiveEntity = entities + size - 1;
		Transform* lastActiveTransform = transforms + size - 1;

		*(entities + index) = *(lastActiveEntity);
		*(transforms + index) = *(lastActiveTransform);

		--size;
	}
}

void TransformManager::GarbageCollect(const EntityManager& entityManager)
{
	// @TODO: Looping over every transform is inefficient. How do you do this better?

	Entity* entityIndex = entities;
	for (size_t i = 0; i < size; ++i, ++entityIndex)
	{
		if (!entityManager.Exists(*entityIndex))
		{
			Remove(*entityIndex);
		}
	}
}

bool TransformManager::Lookup(const Entity entity, size_t* index) const
{
	Entity* entityIndex = entities;
	size_t i = 0;
	for (; i < capacity; ++i, ++entityIndex)
	{
		if (*entityIndex == Entity::null())
		{
			// We hit the end of the index! This entity wasn't in the store!
			return false;
		}
		if (*entityIndex == entity)
		{
			*index = i;
			return true;
		}
	}
	return false;
}

void TransformManager::Allocate(int newCapacity)
{
	capacity = newCapacity;

	// Allocate new memory
	const size_t elementSizeInBytes = sizeof(Entity) + sizeof(Transform);
	void* newBuffer = new size_t[(elementSizeInBytes * newCapacity)];

	// Set up new pointers for where our data will go
	Entity* newEntities = (Entity*)newBuffer;
	Transform* newTransforms = (Transform*)(newEntities + newCapacity);

	if (size > 0)
	{
		// Copy the data to the new buffer
		memcpy(newEntities, entities, sizeof(Entity) * size);
		memcpy(newTransforms, transforms, sizeof(Transform) * size);
	}

	// Switch the pointers around
	entities = newEntities;
	transforms = newTransforms;

	// Switch the buffers and free the old memory
	delete buffer;
	buffer = newBuffer;
}
