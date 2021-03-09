
#include "Create.h"

#include "..\ECS\RigidbodyManager.h"
#include "..\ECS\TransformManager.h"
#include "..\ECS\SpriteManager.h"
#include "..\ECS\EntityManager.h"

#include "..\Math\Math.h"

Create::Create(EntityManager& entities, TransformManager& transforms,
	SpriteManager& sprites, RigidbodyManager& rigidbodies) :
	entityManager(entities),
	transManager(transforms),
	spriteManager(sprites),
	rigidbodyManager(rigidbodies) {}

Entity Create::Asteroid(const Vector2& position, const float& rotation,
	const Vector2& velocity, const float& rotVelocity, const AsteroidType& asteroidType) const
{
	Entity entity = entityManager.Create();

	Transform trans;
	trans.pos = position;
	trans.rot = rotation;
	transManager.Add(entity, trans);
	rigidbodyManager.Add(entity, GetColliderFor(asteroidType), velocity, rotVelocity);
	spriteManager.Create(entity, GetSpriteFor(asteroidType), RenderQueue::Layer::DEFAULT);

	return entity;
}

ColliderType Create::GetColliderFor(const AsteroidType& asteroidType) const
{
	if (asteroidType == AsteroidType::LARGE)
		return ColliderType::LARGE_ASTEROID;

	if(asteroidType < AsteroidType::RANDOM_SMALL)
		return ColliderType::MEDIUM_ASTEROID;
	
	return ColliderType::SMOL_ASTEROID;
}


SpriteID Create::GetSpriteFor(const AsteroidType& asteroidType) const
{
	switch (asteroidType)
	{
	case AsteroidType::LARGE:
		return SpriteID::LARGE_ASTEROID;
		
	case AsteroidType::MEDIUM_1:
		return SpriteID::MEDIUM_ASTEROID_1;
	case AsteroidType::MEDIUM_2:
		return SpriteID::MEDIUM_ASTEROID_2;
	case AsteroidType::MEDIUM_3:
		return SpriteID::MEDIUM_ASTEROID_3;
	case AsteroidType::MEDIUM_4:
		return SpriteID::MEDIUM_ASTEROID_4;

	case AsteroidType::SMALL_1:
		return SpriteID::SMOL_ASTEROID_1;
	case AsteroidType::SMALL_2:
		return SpriteID::SMOL_ASTEROID_2;
	case AsteroidType::SMALL_3:
		return SpriteID::SMOL_ASTEROID_3;
	case AsteroidType::SMALL_4:
		return SpriteID::SMOL_ASTEROID_4;
	case AsteroidType::SMALL_5:
		return SpriteID::SMOL_ASTEROID_5;
	case AsteroidType::SMALL_6:
		return SpriteID::SMOL_ASTEROID_6;
	case AsteroidType::SMALL_7:
		return SpriteID::SMOL_ASTEROID_7;
	case AsteroidType::SMALL_8:
		return SpriteID::SMOL_ASTEROID_8;
	case AsteroidType::SMALL_9:
		return SpriteID::SMOL_ASTEROID_9;
	case AsteroidType::SMALL_10:
		return SpriteID::SMOL_ASTEROID_10;
	case AsteroidType::SMALL_11:
		return SpriteID::SMOL_ASTEROID_11;
	case AsteroidType::SMALL_12:
		return SpriteID::SMOL_ASTEROID_12;
	case AsteroidType::SMALL_13:
		return SpriteID::SMOL_ASTEROID_13;
	case AsteroidType::SMALL_14:
		return SpriteID::SMOL_ASTEROID_14;
	case AsteroidType::SMALL_15:
		return SpriteID::SMOL_ASTEROID_15;
	case AsteroidType::SMALL_16:
		return SpriteID::SMOL_ASTEROID_16;

	case AsteroidType::RANDOM_MEDIUM:
		return (SpriteID)((int)SpriteID::MEDIUM_ASTEROID_1 + Math::RandomRange(0, 4));
	case AsteroidType::RANDOM_SMALL:
		return (SpriteID)((int)SpriteID::SMOL_ASTEROID_1 + Math::RandomRange(0, 16));
	}
}

Entity Create::Ship(const Vector2& position, const float& rotation) const
{
	Entity entity = entityManager.Create();

	Transform trans;
	trans.pos = position;
	trans.rot = rotation;
	transManager.Add(entity, trans);
	spriteManager.Create(entity, SpriteID::SHIP, RenderQueue::Layer::DEFAULT);

	return entity;
}