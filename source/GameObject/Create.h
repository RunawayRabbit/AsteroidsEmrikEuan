#pragma once

class Entity;
class EntityManager;
class TransformManager;
class AsteroidManager;
class SpriteManager;

class Vector2;

class Create
{
public:
	Create(EntityManager& entities, TransformManager& transforms,
		SpriteManager& sprites, AsteroidManager& asteroids);

	Entity Asteroid(const Vector2& position, const float& rotation,
		const Vector2& velocity, const float& rotVelocity);

	Entity Ship(const Vector2& position, const float& rotation);

private:

	EntityManager& entityManager;
	TransformManager& transManager;
	AsteroidManager& asteroidManager;
	SpriteManager& spriteManager;
};