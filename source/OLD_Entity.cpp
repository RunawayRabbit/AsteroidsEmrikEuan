
#include <memory> // for unique_ptr
#include "Entity.h"

#include "Math.h"

Entity::Entity(Sprite* sprite, const Rect* gameField, Vector2 initialPosition, float initialRotationDeg) :
	mySprite(sprite),
	position(initialPosition),
	rotation(initialRotationDeg),
	gameField(gameField),
	renderBounds(gameField->top + mySprite->Height(),
		gameField->bottom - mySprite->Height(),
		gameField->left + mySprite->Width(),
		gameField->right - mySprite->Width())
{
}

void Entity::Displace(Vector2 movement)
{
	position += movement;
	if (!gameField->Contains(position))
	{
		// We're outside of the game world, wrap us to the other side!
		position.x = Math::Repeat(position.x, gameField->right);
		position.y = Math::Repeat(position.y, gameField->bottom);
	}
}

void Entity::PlaceAt(Vector2 movement, float angleDeg)
{
	position = movement;
	rotation = angleDeg;
	if (!gameField->Contains(position))
	{
		// We're outside of the game world, wrap us to the other side!
		position.x = Math::Repeat(position.x, gameField->right);
		position.y = Math::Repeat(position.y, gameField->bottom);
	}
}

void Entity::Draw() const
{
	mySprite->Draw(position, rotation);

	if (!renderBounds.Contains(position))
	{
		// We're at the screen edges. Blit a second sprite so that our wrapping looks smooth.
		Vector2 oppositePosition = position;

		if (position.x < renderBounds.left)
			oppositePosition.x = gameField->right + position.x;
		else if (position.x > renderBounds.right)
			oppositePosition.x = position.x - gameField->right;

		if (position.y < renderBounds.top)
			oppositePosition.y = gameField->bottom + position.y;
		else if (position.y > renderBounds.bottom)
			oppositePosition.y = position.y - gameField->bottom;

		mySprite->Draw(oppositePosition, rotation);
	}
}
