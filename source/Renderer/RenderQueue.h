#pragma once

#include <vector>

#include <SDL_render.h>

class RenderQueue
{
public:
	RenderQueue();

	enum class Layer
	{
		BACKGROUND,
		PARALLAX,
		DEFAULT,
		PARTICLE,
		UI,

		COUNT
	};

	struct Element
	{
		SDL_Texture* tex;
		SDL_Rect srcRect;
		SDL_Rect dstRect;
		float angle;
		RenderQueue::Layer layer;
	};



	void Enqueue(SDL_Texture* texture, const SDL_Rect& sourceRect, const SDL_Rect& targetRect, const float rotation, const RenderQueue::Layer layer);

	const std::vector<RenderQueue::Element>& GetRenderQueue();
	void Clear();



private:
	std::vector<RenderQueue::Element> renderQueue;
};
