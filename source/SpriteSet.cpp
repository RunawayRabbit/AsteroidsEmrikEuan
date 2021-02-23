
#include "SpriteSet.h"


SpriteSet::SpriteSet(Renderer &renderer)
{
	SDL_Rect shipSrc;
	shipSrc.w = 22;
	shipSrc.h = 30;
	shipSrc.x = 5;
	shipSrc.y = 1;

	shipSprite = new Sprite(renderer, shipSrc);

	SDL_Rect asteroidSrc;
	asteroidSrc.w = 58;
	asteroidSrc.h = 61;
	asteroidSrc.x = 66;
	asteroidSrc.y = 194;

	asteroidSprite = new Sprite(renderer, asteroidSrc);
}
SpriteSet::~SpriteSet()
{
	delete asteroidSprite;
	delete shipSprite;
}