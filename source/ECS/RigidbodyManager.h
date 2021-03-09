#pragma once

#include "..\Math\Vector2.h"

#include "Entity.h"
#include "..\Physics\ColliderType.h"

class EntityManager;
class Physics;

struct Rigidbody
{
	Entity entity;
	ColliderType colliderType;
	Vector2 velocity;
	float angularVelocity;

	bool operator<(const Rigidbody& other) const
	{
		return ((int)this->colliderType < (int)other.colliderType);
	}
};

class RigidbodyManager
{
public:
	RigidbodyManager(const EntityManager& entityManager, const int capacity);

	void Allocate(const int capacity);

	void Update(Physics& physics, const float& deltaTime);
	void Add(const Entity& entity, const ColliderType colliderType, const Vector2 velocity, const float rotVelocity);
	bool Lookup(const Entity entity, size_t* index) const;
	bool GetPtr(const Entity entity, Rigidbody** rb);



private:
	int size = 0;
	int capacity;

	void* buffer;
	Entity* entities;
	Rigidbody* rigidbodies;

	const EntityManager& entityManager;
};