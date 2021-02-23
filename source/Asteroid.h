#pragma once

#include "Entity.h"
#include "Vector2.h"

class Asteroid : public Entity
{
public:
	Asteroid(SpriteAtlas& spriteAtlas, const Rect* gameField, Vector2 initialPosition, float initialRotationDeg);
	Asteroid() = delete;

	void Update(float deltaTime);

private:
	Vector2 velocity;
	float rotVelocity;
};