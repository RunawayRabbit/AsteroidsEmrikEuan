#pragma once

#include "..\Physics\ColliderType.h"
#include "..\Renderer\SpriteAtlas.h"

class Entity;
class EntityManager;
class TransformManager;
class RigidbodyManager;
class SpriteManager;

class Vector2;

class Create
{
public:
	enum class AsteroidType
	{
		LARGE,

		RANDOM_MEDIUM,
		MEDIUM_1,
		MEDIUM_2,
		MEDIUM_3,
		MEDIUM_4,

		RANDOM_SMALL,
		SMALL_1,
		SMALL_2,
		SMALL_3,
		SMALL_4,
		SMALL_5,
		SMALL_6,
		SMALL_7,
		SMALL_8,
		SMALL_9,
		SMALL_10,
		SMALL_11,
		SMALL_12,
		SMALL_13,
		SMALL_14,
		SMALL_15,
		SMALL_16,
	};

	Create(EntityManager& entities, TransformManager& transforms,
		SpriteManager& sprites, RigidbodyManager& rigidbodies);

	Entity Asteroid(const Vector2& position, const float& rotation,
		const Vector2& velocity, const float& rotVelocity, const AsteroidType& asteroidType) const;

	Entity Ship(const Vector2& position, const float& rotation) const;

private:

	ColliderType GetColliderFor(const AsteroidType& asteroidType) const;
	SpriteID GetSpriteFor(const AsteroidType& asteroidType) const;

	EntityManager& entityManager;
	TransformManager& transManager;
	RigidbodyManager& rigidbodyManager;
	SpriteManager& spriteManager;
};