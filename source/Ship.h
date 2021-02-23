#pragma once

#include "Entity.h"
#include "Vector2.h"

class Ship : public Entity
{
public:
	Ship(SpriteAtlas& spriteAtlas, const Rect* gameField, Vector2 initialPosition, float initialRotationDeg);
	Ship() = delete;

	void Update(float deltaTime);

private:
	Vector2 velocity;
};