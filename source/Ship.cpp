
#include "Ship.h"
#include "Math.h"

Ship::Ship(SpriteAtlas& spriteAtlas, const Rect* gameField, Vector2 initialPosition, float initialRotationDeg) :
	Entity(spriteAtlas.shipSprite, gameField, initialPosition, initialRotationDeg)
{
	float x = Math::RandomRange(-10.0f, 10.0f);
	float y = Math::RandomRange(-10.0f, 10.0f);

	velocity = { x, y };
}

void Ship::Update(float deltaTime) // @TODO: This should take the player input struct?
{
	Displace(velocity * deltaTime);
}