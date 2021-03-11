#pragma once

#include <unordered_map>

#include "Transform.h"
#include "Entity.h"
#include "EntityManager.h"

class TransformManager
{
public:
	TransformManager(int capacity);

	bool Get(const Entity entity, Transform& transform) const;
	bool GetMutable(const Entity entity, Transform*& transform);

	void Add(const Entity entity, const Transform transform);
	void GarbageCollect(const EntityManager& entityManager);

private:
	std::unordered_map<Entity, Transform> transforms;

};