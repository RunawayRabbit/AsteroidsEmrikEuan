#pragma once

class Entity;
class EntityManager;
class TransformManager;
class RigidbodyManager;
class SpriteManager;

class Vector2;

class Create
{
public:
	Create(EntityManager& entities, TransformManager& transforms,
		SpriteManager& sprites, RigidbodyManager& rigidbodies);

	Entity Asteroid(const Vector2& position, const float& rotation,
		const Vector2& velocity, const float& rotVelocity);

	Entity Ship(const Vector2& position, const float& rotation);

private:

	EntityManager& entityManager;
	TransformManager& transManager;
	RigidbodyManager& rigidbodyManager;
	SpriteManager& spriteManager;
};