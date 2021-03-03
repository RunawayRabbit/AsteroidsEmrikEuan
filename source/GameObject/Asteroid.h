#pragma once

#include "..\ECS\Entity.h"
#include "..\Math\Vector2.h"
#include "..\Math\AABB.h"

class Asteroid : public Entity
{
public:
	Asteroid(SpriteAtlas& spriteAtlas, const AABB* gameField, Vector2 initialPosition, float initialRotationDeg);
	Asteroid() = delete;

	void Update(float deltaTime);

private:
	Vector2 velocity;
	float rotVelocity;
};