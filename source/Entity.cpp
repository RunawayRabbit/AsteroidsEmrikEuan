
#include <memory> // for unique_ptr
#include "Entity.h"
#include "Asteroid.h"

Entity::Entity(Sprite* sprite, Vector2 initialPosition, float initialRotationDeg) :
	mySprite(sprite),
	position(initialPosition),
	rotation(initialRotationDeg)
{
}

std::unique_ptr<Entity> Entity::Make(Entity::Type type, SpriteSet& spriteSet, Vector2 initialPosition, float initialRotationDeg)
{
	switch (type)
	{
	case Type::Asteroid:		
		return std::make_unique<Asteroid>(spriteSet.asteroidSprite, initialPosition, initialRotationDeg);

	case Type::Ship:
		return nullptr; // STUB
	}

	return nullptr;


}

void Entity::Draw() const
{
	mySprite->Draw(position, rotation);
}
