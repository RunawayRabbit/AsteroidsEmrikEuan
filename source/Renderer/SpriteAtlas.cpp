
#include <iostream> // @TODO: We need a logging singleton to stop this shit from happening..

#include <SDL_image.h>

#include "..\Renderer\Renderer.h"

#include "SpriteAtlas.h"

SpriteAtlas::SpriteAtlas(Renderer& renderer) : 
	spriteData((int)SpriteID::COUNT)
{
	LoadPNGs(renderer.GetRenderer());

	spriteData[(int)SpriteID::NONE]; // null data, default, nothing, nadda, zip.

	CreateAnimatedSprites();
	CreateRegularSprites();
}

void SpriteAtlas::CreateAnimatedSprites()
{
	//@TODO: @BUG:
	/*
	We aren't doing anything close to intelligent when it comes to tracking animated sprites.
	They will clobber each other in this arrangement, although I *do* want this data layout.

	For now, I'm going to have each animated sprite be positioned explicitly in the enum as
	being previousSprite + previousSpriteFrameCount. But that is janky, error-prone and all
	round terrible. I need a better solution but I can't think of one right now.

	*/

	CreateSprite(SpriteID::SHIP_TRAIL, 0, 8, 15, 76, 48, 0);
	CreateSprite(SpriteID::SHIP_TRAIL, 0, 8, 15, 108, 48, 1);
	CreateSprite(SpriteID::SHIP_TRAIL, 0, 8, 15, 140, 48, 2);
	CreateSprite(SpriteID::SHIP_TRAIL, 0, 8, 15, 172, 48, 3);
}

void SpriteAtlas::CreateRegularSprites()
{
	CreateSprite(SpriteID::SHIP, 0, 25, 23, 3, 34);
	CreateSprite(SpriteID::ASTEROID, 0, 58, 61, 66, 194);
	CreateSprite(SpriteID::SHITTY_LOGO, 1, 400, 114, 0, 0);
}

void SpriteAtlas::CreateSprite(SpriteID id, int texIndex, int width, int height, int x, int y, int animationFrame)
{
	Sprite sprite;
	sprite.id = id; //@TODO: Redundant?
	sprite.texture = loadedImages[texIndex]; // resources/asteroids-arcade.png
	sprite.source.w = width;
	sprite.source.h = height;
	sprite.source.x = x;
	sprite.source.y = y;

	spriteData[(int)sprite.id + animationFrame] = sprite;
}

void SpriteAtlas::LoadPNGs(SDL_Renderer* renderer)
{
	// Perform ALL Image loading Here!
	if (!(IMG_Init(IMG_INIT_PNG) & IMG_INIT_PNG))
	{
		std::cout << ("Error initializing PNG extensions: ") << SDL_GetError();
	}

	loadedImages.push_back(PNGToTexture(renderer, "resources/asteroids-arcade.png"));
	loadedImages.push_back(PNGToTexture(renderer, "resources/crappy_logo.png"));

	IMG_Quit(); // Shut down the image loading stuff, we don't need it anymore.
}

SDL_Texture* SpriteAtlas::PNGToTexture(SDL_Renderer* renderer, const std::string path) const
{
	SDL_Surface* surf = IMG_Load(path.c_str());

	if (!surf)
	{
		std::cout << ("Failed to load " + path + ".\n") << SDL_GetError();
	}

	SDL_Texture* tex = SDL_CreateTextureFromSurface(renderer, surf);
	if (!tex)
	{
		SDL_FreeSurface(surf);
		SDL_DestroyTexture(tex);

		std::cout << ("Failed to convert " + path + " to a texture.\n") << SDL_GetError();
	}

	SDL_FreeSurface(surf);

	return tex;
}