
#include "Asteroid.h"
#include "Math.h"

Asteroid::Asteroid(Sprite* sprite, Vector2 initialPosition, float initialRotationDeg) :
	Entity(sprite, initialPosition, initialRotationDeg)
{
	float x = Math::RandomRange(-10.0f, 10.0f);
	float y = Math::RandomRange(-10.0f, 10.0f);

	velocity = { x, y };
	rotVelocity = Math::RandomRange(-10.0f, 10.0f);
}

void Asteroid::Update(float deltaTime)
{
	position += velocity * deltaTime;
	rotation += rotVelocity * deltaTime;
}