
#include "AnimatedSprite.h"
#include <algorithm>

AnimatedSprite::AnimatedSprite(const Renderer& renderer, std::vector<SDL_Rect> positions) :
	Sprite(renderer, positions.front()),
	frames(positions)
{
}

void AnimatedSprite::Draw(Vector2 pos, float angle, int frame) const
{
	int frameToRender = std::clamp(frame, 0, (int)frames.size());

	this->Sprite::Draw(pos, angle, frames.at(frameToRender));
}
