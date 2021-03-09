#pragma once

#include <array>

#include "Transform.h"
#include "Entity.h"
#include "EntityManager.h"

class TransformManager
{
public:
	TransformManager(int capacity);
	~TransformManager();

	bool Get(const Entity entity, Transform* transform) const;
	bool GetPtr(const Entity entity, Transform** transform);

	bool Set(const Entity entity, const Transform transform);
	void Add(const Entity entity, const Transform transform);
	void Remove(const Entity entity);
	void GarbageCollect(const EntityManager& entityManager);

private:
	int size;
	int capacity;

	Entity* entities;
	Transform* transforms;

	void* buffer; // Raw pointer we got back from malloc, where all our shit is

	void Allocate(int newCapacity);
	bool Lookup(const Entity entity, size_t* index) const;

	std::vector<Entity> dirtyList;
};