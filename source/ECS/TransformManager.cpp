
#include "TransformManager.h"
#include "EntityManager.h"

#include "..\Physics\Move.h"

TransformManager::TransformManager(int capacity)
{
	_size = 0;
	Allocate(capacity);
}

TransformManager::~TransformManager()
{
	free(_buffer);
}

bool TransformManager::Get(const Entity entity, Transform* transform) const
{
	size_t index;
	if (Lookup(entity, &index))
	{
		*transform = *(_transforms + index);
		return true;
	}
	return false;
}

bool TransformManager::Set(const Entity entity, const Transform transform)
{
	size_t index;
	if (Lookup(entity, &index))
	{
		*(_transforms + index) = transform;
		return true;
	}
	return false;
}

void TransformManager::Add(const Entity entity, const Transform transform)
{
	if (_size == _capacity)
	{
		// We're about to overrun our buffer, we gotta scale.
		Allocate((size_t)(_size * (size_t)2));
	}

#if DEBUG_STUFF


	if (auto index = Lookup(entity))
	{
		// Tried to add multiple transforms to the same entity. We don't allow that!
		_ASSERT(false);
	}

#endif

	// Insert our data at the back of the data store
	*(_entities + _size) = entity;
	*(_transforms + _size) = transform;

	++_size;
}

void TransformManager::Remove(const Entity entity)
{
	size_t index;
	if (Lookup(entity, &index))
	{
		// swap our entity to the back
		Entity* lastActiveEntity = _entities + _size - 1;
		Transform* lastActiveTransform = _transforms + _size - 1;

		*(_entities + index) = *(lastActiveEntity);
		*(_transforms + index) = *(lastActiveTransform);

		--_size;
	}
}

void TransformManager::GarbageCollect(const EntityManager& entityManager)
{
	// @TODO: Looping over every transform is inefficient. How do you do this better?

	Entity* entityIndex = _entities;
	for (size_t i = 0; i < _size; ++i, ++entityIndex)
	{
		if (!entityManager.Exists(*entityIndex))
		{
			Remove(*entityIndex);
		}
	}
}

bool TransformManager::Lookup(const Entity entity, size_t* index) const
{
	Entity* entityIndex = _entities;
	size_t i = 0;
	for (; i < _capacity; ++i, ++entityIndex)
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
	_capacity = newCapacity;

	// Allocate new memory
	const size_t elementSizeInBytes = sizeof(Entity) + sizeof(Transform);
	void* newBuffer = new size_t[(elementSizeInBytes * newCapacity)];

	// Set up new pointers for where our data will go
	Entity* newEntities = (Entity*)newBuffer;
	Transform* newTransforms = (Transform*)(newEntities + newCapacity);

	if (_size > 0)
	{
		// Copy the data to the new buffer
		memcpy(newEntities, _entities, sizeof(Entity) * _size);
		memcpy(newTransforms, _transforms, sizeof(Transform) * _size);
	}

	// Switch the pointers around
	_entities = newEntities;
	_transforms = newTransforms;

	// Switch the buffers and free the old memory
	delete _buffer;
	_buffer = newBuffer;
}

void TransformManager::Update(const std::vector<Move> resolvedCollisions)
{
	for (auto& resolvedCollision : resolvedCollisions)
	{
		Entity ent;
	}
}

const std::vector<Entity> TransformManager::GetDirtyList() const 
{
	return dirtyList;
}