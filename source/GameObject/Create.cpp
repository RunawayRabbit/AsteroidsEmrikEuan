
#include "Create.h"

#include "..\ECS\AsteroidManager.h"
#include "..\ECS\TransformManager.h"
#include "..\ECS\SpriteManager.h"
#include "..\ECS\EntityManager.h"

Create::Create(EntityManager& entities, TransformManager& transforms,
	SpriteManager& sprites, AsteroidManager& asteroids) :
	entityManager(entities),
	transManager(transforms),
	spriteManager(sprites),
	asteroidManager(asteroids) {}

Entity Create::Asteroid(const Vector2& position, const float& rotation,
	const Vector2& velocity, const float& rotVelocity)
{
	Entity entity = entityManager.Create();

	Transform trans;
	trans.pos = position;
	trans.rot = rotation;
	transManager.Add(entity, trans);
	spriteManager.Create(entity, SpriteID::ASTEROID, RenderQueue::Layer::DEFAULT);
	asteroidManager.Add(entity, velocity, rotVelocity);

	return entity;
}

Entity Create::Ship(const Vector2& position, const float& rotation)
{
	Entity entity = entityManager.Create();

	Transform trans;
	trans.pos = position;
	trans.rot = rotation;
	transManager.Add(entity, trans);
	spriteManager.Create(entity, SpriteID::SHIP, RenderQueue::Layer::DEFAULT);

	return entity;
}