#pragma once

#include <SDL_render.h>

#include "Vector2.h"
#include "Renderer.h"

class Sprite
{
public:
	Sprite(const Renderer& renderer, SDL_Rect position);

	int Width() const { return source.w; };
	int Height() const { return source.h; };

	void Draw(Vector2 pos, float angle) const;

private:
	const SDL_Rect source;
	const SDL_Texture* texture;
	const Renderer* render;
};

