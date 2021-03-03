#pragma once

#include <array>

#include "Transform.h"
#include "Entity.h"
#include "EntityManager.h"

#include "..\Physics\Move.h"

class TransformManager
{
public:
	TransformManager(int capacity);
	~TransformManager();

	bool Get(const Entity entity, Transform* transform) const;
	bool Set(const Entity entity, const Transform transform);
	void Add(const Entity entity, const Transform transform);
	void Remove(const Entity entity);
	void GarbageCollect(const EntityManager& entityManager);

	void Update(const std::vector<Move>);
	const std::vector<Entity> GetDirtyList() const;

private:
	size_t _size;
	size_t _capacity;

	Entity* _entities;
	Transform* _transforms;

	void* _buffer; // Raw pointer we got back from malloc, where all our shit is

	void Allocate(int newCapacity);
	bool Lookup(const Entity entity, size_t* index) const;

	std::vector<Entity> dirtyList;
};