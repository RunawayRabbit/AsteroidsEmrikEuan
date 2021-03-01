#pragma once

#include <SDL_render.h>

#include "../Math/Vector2.h"
#include "Renderer.h"

class Sprite
{
public:
	Sprite(const Renderer& renderer, SDL_Rect position);

	int Width() const { return source.w; };
	int Height() const { return source.h; };

	virtual void Draw(Vector2 pos, float angle) const;


protected:
	virtual void Draw(Vector2 pos, float angle, SDL_Rect source) const;

private:
	const SDL_Rect source;
	const SDL_Texture* texture;
	const Renderer* render;
};

