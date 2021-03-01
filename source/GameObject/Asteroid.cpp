
#include "Asteroid.h"
#include "Math.h"

Asteroid::Asteroid(SpriteAtlas& spriteAtlas, const Rect* gameField, Vector2 initialPosition, float initialRotationDeg) :
	Entity(spriteAtlas.asteroidSprite, gameField, initialPosition, initialRotationDeg)
{
	float x = Math::RandomRange(-70.0f, 70.0f);
	float y = Math::RandomRange(-70.0f, 70.0f);

	velocity = { x, y };
	rotVelocity = Math::RandomRange(-40.0f, 40.0f);
}

void Asteroid::Update( float deltaTime)
{
	Displace(velocity * deltaTime);
	rotation += rotVelocity * deltaTime;
}