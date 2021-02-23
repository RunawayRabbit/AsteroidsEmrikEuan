
#include "Asteroid.h"

Asteroid::Asteroid(Sprite* sprite, Vector2 initialPosition, float initialRotationDeg) :
	Entity(sprite, initialPosition, initialRotationDeg)
{
	velocity = Vector2::zero();
	rotVelocity = 0.0f;
}

void Asteroid::Update(float deltaTime)
{
	position += velocity * deltaTime;
	rotation += rotVelocity * deltaTime;
}