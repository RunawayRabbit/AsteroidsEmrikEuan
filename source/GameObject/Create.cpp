
#include "Create.h"

#include "..\ECS\RigidbodyManager.h"
#include "..\ECS\TransformManager.h"
#include "..\ECS\SpriteManager.h"
#include "..\ECS\EntityManager.h"

Create::Create(EntityManager& entities, TransformManager& transforms,
	SpriteManager& sprites, RigidbodyManager& rigidbodies) :
	entityManager(entities),
	transManager(transforms),
	spriteManager(sprites),
	rigidbodyManager(rigidbodies) {}

Entity Create::Asteroid(const Vector2& position, const float& rotation,
	const Vector2& velocity, const float& rotVelocity)
{
	Entity entity = entityManager.Create();

	Transform trans;
	trans.pos = position;
	trans.rot = rotation;
	transManager.Add(entity, trans);
	spriteManager.Create(entity, SpriteID::LARGE_ASTEROID, RenderQueue::Layer::DEFAULT);
	rigidbodyManager.Add(entity, ColliderType::LARGE_ASTEROID, velocity, rotVelocity);

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