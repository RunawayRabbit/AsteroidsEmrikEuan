#pragma once

#include "Entity.h"
#include "Vector2.h"

class Asteroid : public Entity
{
public:
	Asteroid(Sprite* sprite, Vector2 initialPosition, float initialRotationDeg);
	void Update(float deltaTime);

private:
	Vector2 velocity;
	float rotVelocity;
};