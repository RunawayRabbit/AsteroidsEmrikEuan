#pragma once

#include "Sprite.h"

class SpriteSet
{
public:
	SpriteSet(Renderer &renderer);
	SpriteSet() = delete;
	~SpriteSet();

	Sprite* asteroidSprite;
	Sprite* shipSprite;
};

