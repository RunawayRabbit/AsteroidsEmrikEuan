#pragma once

#include "Sprite.h"
#include "AnimatedSprite.h"

class SpriteAtlas
{
public:
	SpriteAtlas(Renderer &renderer);
	SpriteAtlas() = delete;
	~SpriteAtlas();

	Sprite* asteroidSprite;
	Sprite* shipSprite;
	AnimatedSprite* shipTrailSprite;
};

