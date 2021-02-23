#pragma once

#include <vector>
#include "Sprite.h"


class AnimatedSprite : public Sprite
{
public:
	AnimatedSprite(const Renderer& renderer, std::vector<SDL_Rect> positions);
	AnimatedSprite() = delete;

	void Draw(Vector2 pos, float angle, int frame = 0) const;

private:
	std::vector<SDL_Rect> frames;
};