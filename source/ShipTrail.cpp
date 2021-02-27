
#include "ShipTrail.h"


ShipTrail::ShipTrail(const SpriteAtlas* atlas, const Rect* gameField,
	Vector2 initialPosition, float initialRotationDeg, Ship& ourShip) : 
	Entity(atlas->shipTrailSprite, gameField, initialPosition, initialRotationDeg)
{
	//spriteAtlas.shipTrailSprite->Draw({ 300, 300 }, 0.0f, animTest % 4);
}

void ShipTrail::PlaceAt(Vector2 pos, Vector2 angle)
{
	PlaceAt(pos, angle);
}

