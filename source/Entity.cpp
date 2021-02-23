
#include <memory> // for unique_ptr
#include "Entity.h"
#include "Asteroid.h"

Entity::Entity(Sprite* sprite, Vector2 initialPosition, float initialRotationDeg) :
	mySprite(sprite),
	position(initialPosition),
	rotation(initialRotationDeg)
{
}

void Entity::Draw() const
{
	mySprite->Draw(position, rotation);
}
