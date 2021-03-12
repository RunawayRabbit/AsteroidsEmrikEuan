
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

std::array<Entity, 4> Create::SplitAsteroid(const Entity& asteroid, const float& splitImpulse) const
{
	std::array<Entity, 4> retVal = { Entity::null(), Entity::null(), Entity::null(), Entity::null() };

	Rigidbody* parentRigid;
	if (!rigidbodyManager.GetMutable(asteroid, parentRigid))
	{
		return retVal;
	}

	std::array<SpriteID, 4> sprites;
	ColliderType colliderType;
	float parentRadius;

	switch (parentRigid->colliderType)
	{
	case ColliderType::LARGE_ASTEROID:
		sprites.at(0) = SpriteID::MEDIUM_ASTEROID_1;
		sprites.at(1) = SpriteID::MEDIUM_ASTEROID_2;
		sprites.at(2) = SpriteID::MEDIUM_ASTEROID_3;
		sprites.at(3) = SpriteID::MEDIUM_ASTEROID_4;
		colliderType = ColliderType::MEDIUM_ASTEROID;
		parentRadius = ColliderRadius::Large;
		break;

	case ColliderType::MEDIUM_ASTEROID:
		sprites.at(0) = (SpriteID)Math::RandomRange((int)SpriteID::SMOL_ASTEROID_1, (int)SpriteID::SMOL_ASTEROID_16);
		sprites.at(1) = (SpriteID)Math::RandomRange((int)SpriteID::SMOL_ASTEROID_1, (int)SpriteID::SMOL_ASTEROID_16);
		sprites.at(2) = (SpriteID)Math::RandomRange((int)SpriteID::SMOL_ASTEROID_1, (int)SpriteID::SMOL_ASTEROID_16);
		sprites.at(3) = (SpriteID)Math::RandomRange((int)SpriteID::SMOL_ASTEROID_1, (int)SpriteID::SMOL_ASTEROID_16);
		colliderType = ColliderType::SMOL_ASTEROID;
		parentRadius = ColliderRadius::Medium;
		break;

	default:
		// Only LARGE_ASTEROID and MEDIUM_ASTEROID are splittable.
		return retVal;
	}

	Transform* parentTransform;
	if (!transManager.GetMutable(asteroid, parentTransform))
	{
		return retVal;
	}

	Vector2 halfParentForward = Vector2::Forward().RotateDeg(parentTransform->rot) * 0.5f;
	Vector2 halfParentRight = halfParentForward.Rot90CW();

	std::array<Vector2, 4> directions;

	directions.at(0) = (-halfParentForward + halfParentRight);
	directions.at(1) = (-halfParentForward - halfParentRight);
	directions.at(2) = (halfParentForward + halfParentRight);
	directions.at(3) = (halfParentForward - halfParentRight);


	for (auto i = 0; i < 4; i++)
	{
		Entity entity = entityManager.Create();
		
		Transform trans;
		trans.pos = parentTransform->pos + (directions.at(i) * (parentRadius + 0.0001f));
		trans.rot = parentTransform->rot;
		transManager.Add(entity, trans);
		rigidbodyManager.Add(entity, colliderType, parentRigid->velocity + (directions.at(i) * splitImpulse), parentRigid->angularVelocity);
		spriteManager.Create(entity, sprites.at(i), RenderQueue::Layer::DEFAULT);
		
		retVal.at(i) = entity;
	}

	entityManager.Destroy(asteroid);

	return retVal;
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
		return (SpriteID)((int)SpriteID::MEDIUM_ASTEROID_1 + Math::RandomRange(0, 3));
	case AsteroidType::RANDOM_SMALL:
		return (SpriteID)((int)SpriteID::SMOL_ASTEROID_1 + Math::RandomRange(0, 15));
	}
}


Entity Create::Bullet(const Vector2& position, const float& rotation, const float& speed, const float& secondsToLive) const
{
	Entity entity = entityManager.Create();

	Transform trans;
	trans.pos = position;
	trans.rot = rotation + 180;
	transManager.Add(entity, trans);
	spriteManager.Create(entity, SpriteID::BULLET, RenderQueue::Layer::PARTICLE);	
	rigidbodyManager.Add(entity, ColliderType::BULLET, -Vector2::Forward().RotateDeg(rotation)* speed, 0);

	entityManager.DestroyDelayed(entity, secondsToLive);

	return entity;
}

Entity Create::Ship(const Vector2& position, const float& rotation, const Vector2& initialVelocity, const float& initialAngularVelocity) const
{
	Entity entity = entityManager.Create();

	Transform trans;
	trans.pos = position;
	trans.rot = rotation;
	transManager.Add(entity, trans);
	spriteManager.Create(entity, SpriteID::SHIP, RenderQueue::Layer::DEFAULT);
	rigidbodyManager.Add(entity, ColliderType::SHIP, initialVelocity, initialAngularVelocity);

	return entity;
}

Entity Create::ShipThruster(const Entity& ship, const Vector2& thrusterOffset, const float& thrusterRotation, SpriteID spriteID) const
{
	Transform parentTrans;
	if (!transManager.Get(ship, parentTrans))
	{
		return Entity::null();
	}

	Entity entity = entityManager.Create();

	Transform trans;
	trans.pos = parentTrans.pos + thrusterOffset.RotateDeg(parentTrans.rot);
	trans.rot = parentTrans.rot;
	transManager.Add(entity, trans);
	spriteManager.Create(entity, spriteID, RenderQueue::Layer::PARTICLE);

	return entity;
}