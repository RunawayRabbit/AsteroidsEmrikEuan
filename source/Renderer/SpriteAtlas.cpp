
#include "SpriteAtlas.h"


SpriteAtlas::SpriteAtlas(Renderer &renderer)
{
	//
	//	BASIC SPRITES
	//

	// SHIP

	SDL_Rect shipSrc;
	shipSrc.w = 25;
	shipSrc.h = 23;
	shipSrc.x = 3;
	shipSrc.y = 34;
	shipSprite = new Sprite(renderer, shipSrc);


	// ASTEROID

	SDL_Rect asteroidSrc;
	asteroidSrc.w = 58;
	asteroidSrc.h = 61;
	asteroidSrc.x = 66;
	asteroidSrc.y = 194;
	asteroidSprite = new Sprite(renderer, asteroidSrc);

	//
	// ANIMATED SPRITES
	//

	// SHIP TRAIL

	std::vector<SDL_Rect> shipTrailSrc(4);

	// Frame 0
	shipTrailSrc[0].w = 8;
	shipTrailSrc[0].h = 15;
	shipTrailSrc[0].x = 76;
	shipTrailSrc[0].y = 48;

	// Frame 1
	shipTrailSrc[1].w = 8;
	shipTrailSrc[1].h = 15;
	shipTrailSrc[1].x = 108;
	shipTrailSrc[1].y = 48;

	// Frame 2
	shipTrailSrc[2].w = 8;
	shipTrailSrc[2].h = 15;
	shipTrailSrc[2].x = 140;
	shipTrailSrc[2].y = 48;

	// Frame 3	 
	shipTrailSrc[3].w = 8;
	shipTrailSrc[3].h = 15;
	shipTrailSrc[3].x = 172;
	shipTrailSrc[3].y = 48;				  

	shipTrailSprite = new AnimatedSprite(renderer, shipTrailSrc);

}
SpriteAtlas::~SpriteAtlas()
{
	delete asteroidSprite;
	delete shipSprite;
	delete shipTrailSprite;
}