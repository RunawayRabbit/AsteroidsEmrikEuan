#pragma once

#include "Entity.h"
#include "Ship.h"
#include "SpriteAtlas.h"
#include "Vector2.h"

class ShipTrail : public Entity
{
public:

	ShipTrail() = delete;
	ShipTrail(const SpriteAtlas* atlas, const Rect* gameField, Vector2 initialPosition, float initialRotationDeg,
		Ship& ourShip);

	void PlaceAt(Vector2 pos, Vector2 angle);

private:
	float burnTime = 0;

	static constexpr float animationSpeed = 0.3f;
	//spriteAtlas.shipTrailSprite->Draw({ 300, 300 }, 0.0f, animTest % 4);
};


