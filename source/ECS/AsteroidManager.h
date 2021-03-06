#pragma once

#include "..\Math\Vector2.h"
#include "..\Physics\Physics.h"

#include "Entity.h"

class EntityManager;

struct Asteroid
{
	Vector2 velocity;
	float angularVelocity;
};

class AsteroidManager
{
public:
	AsteroidManager(const EntityManager& entityManager, const int capacity);

	void Allocate(const int capacity);

	void Update(Physics& physics, const float& deltaTime);
	void Add(const Entity& entity, const Vector2 velocity, const float rotVelocity);


private:
	int size = 0;
	int capacity;

	void* buffer;
	Entity* entities;
	Asteroid* asteroids;

	const EntityManager& entityManager;
};