#pragma once

#include <memory>

#include "SpriteAtlas.h"
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

	float rotation;

private:
	Sprite* mySprite;

	Vector2 position; // No direct access for children. Use Displace() instead. (Required to make toroidal scrolling work.)

	const Rect* gameField;
	const Rect renderBounds;
};