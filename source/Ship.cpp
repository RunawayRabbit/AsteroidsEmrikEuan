
#include "Ship.h"
#include "Math.h"

Ship::Ship(SpriteSet& spriteSet, Vector2 initialPosition, float initialRotationDeg) :
	Entity(spriteSet.shipSprite, initialPosition, initialRotationDeg)
{
	float x = Math::RandomRange(-10.0f, 10.0f);
	float y = Math::RandomRange(-10.0f, 10.0f);

	velocity = { x, y };
}

void Ship::Update(float deltaTime) // @TODO: This should take the player input struct
{
	position += velocity * deltaTime;
}