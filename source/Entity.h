#pragma once

#include <memory>

#include "SpriteSet.h"
#include "Vector2.h"


class Entity
{
public:
	enum class Type
	{
		Ship,
		Asteroid,
	};

	Entity() = delete;

	//static std::unique_ptr<Entity> Make(Entity::Type type, SpriteSet& spriteSet, Vector2 initialPosition, float initialRotationDeg);

	void Draw() const;
	virtual void Update(float deltaTime) = 0;

protected:
	Entity(Sprite* sprite, Vector2 initialPosition, float initialRotationDeg);
	Vector2 position;
	float rotation;

private:
	Sprite* mySprite;
};