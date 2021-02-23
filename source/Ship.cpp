
#include "Ship.h"
#include "Math.h"

Ship::Ship(SpriteSet& spriteSet, const Rect* gameField, Vector2 initialPosition, float initialRotationDeg) :
	Entity(spriteSet.shipSprite, gameField, initialPosition, initialRotationDeg)
{
	float x = Math::RandomRange(-10.0f, 10.0f);
	float y = Math::RandomRange(-10.0f, 10.0f);

	velocity = { x, y };
}

void Ship::Update(float deltaTime) // @TODO: This should take the player input struct
{
	Displace(velocity * deltaTime);
}