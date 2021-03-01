
#include "Sprite.h"
#include <math.h> // for round

Sprite::Sprite(const Renderer& renderer, SDL_Rect position) : 
	source(position),
	render(&renderer),
	texture(renderer.GetSpritesheet())
{
}

void Sprite::Draw(Vector2 pos, float angle) const
{
	SDL_Rect dest;
	dest.w = source.w;
	dest.h = source.h;
	dest.x = (int)roundf(pos.x);
	dest.y = (int)roundf(pos.y);

	SDL_RenderCopyEx(render->GetRenderer(),
		(SDL_Texture*)texture, &source, &dest, angle,
		NULL, SDL_FLIP_NONE);
}

void Sprite::Draw(Vector2 pos, float angle, SDL_Rect source) const
{
	SDL_Rect dest;
	dest.w = source.w;
	dest.h = source.h;
	dest.x = (int)roundf(pos.x);
	dest.y = (int)roundf(pos.y);

	SDL_RenderCopyEx(render->GetRenderer(),
		(SDL_Texture*)texture, &source, &dest, angle,
		NULL, SDL_FLIP_NONE);
}