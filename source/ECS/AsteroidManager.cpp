
#include "AsteroidManager.h"
#include "EntityManager.h"

AsteroidManager::AsteroidManager(const EntityManager& entityManager, const int capacity) :
	entityManager(entityManager)
{
	Allocate(capacity);
}

void AsteroidManager::Add(const Entity& entity, const Vector2 velocity, const float rotVelocity)
{
	if (size == capacity)
	{
		// We're about to overrun our buffer, we gotta scale.
		Allocate((size_t)(size * (size_t)2));
	}

	// Insert our data at the back of the data store
	Asteroid asteroid;
	asteroid.velocity = velocity;
	asteroid.angularVelocity = rotVelocity;

	*(entities + size) = entity;
	*(asteroids + size) = asteroid;

	++size;
}

void AsteroidManager::Allocate(const int newCapacity)
{
	capacity = newCapacity;

	// Allocate new memory
	const size_t elementSizeInBytes = sizeof(Entity) + sizeof(Asteroid);
	void* newBuffer = new size_t[(elementSizeInBytes * newCapacity)];

	// Set up new pointers for where our data will go
	Entity* newEntities = (Entity*)newBuffer;
	Asteroid* newAsteroids = (Asteroid*)(newEntities + newCapacity);

	if (size > 0)
	{
		// Copy the data to the new buffer
		memcpy(newEntities, entities, sizeof(Entity) * size);
		memcpy(newAsteroids, asteroids, sizeof(Asteroid) * size);
	}

	// Switch the pointers around
	entities = newEntities;
	asteroids = newAsteroids;

	// Switch the buffers and free the old memory
	delete buffer;
	buffer = newBuffer;
}


void AsteroidManager::Update(Physics& physics, const float& deltaTime)
{
	int i = 0;
	while (i < size)
	{
		Entity* entity = (entities + i);
		Asteroid* asteroid = (asteroids + i);

		Transform transform;
		if (entityManager.Exists(*entity))
		{
			physics.Enqueue(*entity, asteroid->velocity, asteroid->angularVelocity);
			++i;
		}
		else
		{
			// Transform appears to have been deleted.
			// Do the swap to remove it from the list.
			Entity* lastActiveEntity = entities + size - 1;
			Asteroid* lastActiveAsteroid = asteroids + size - 1;

			*(entities + i) = *(lastActiveEntity);
			*(asteroids + i) = *(lastActiveAsteroid);

			--size;
		}
	}
}