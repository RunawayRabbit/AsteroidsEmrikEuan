#pragma once

#include <memory>

#include "SpriteSet.h"
#include "Vector2.h"
#include "Rect.h"


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
	Entity(Sprite* sprite, const Rect* gameField, Vector2 initialPosition, float initialRotationDeg);

	void Displace(Vector2 movement);

	Vector2 position;
	float rotation;

private:
	Sprite* mySprite;
	const Rect* gameField;
	const Rect renderBounds;
};