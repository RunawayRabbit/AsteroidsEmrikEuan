#pragma once

#include <vector>

#include <SDL_render.h>

class RenderQueue
{
public:
	RenderQueue();

	struct Element
	{
		SDL_Texture* tex; // 64
		SDL_Rect srcRect; // 128
		SDL_Rect dstRect; // 128
		float angle;
		// total 320 bits
	};

	enum class Layer
	{
		BACKGROUND,
		PARALLAX,
		ASTEROID,
		SHIP,
		PARTICLE,
		UI,

		COUNT
	};

	void Enqueue(SDL_Texture* texture, const SDL_Rect& sourceRect, const SDL_Rect& targetRect, RenderQueue::Layer layer);

	const std::vector<RenderQueue::Element>& GetRenderQueue() const;
	void Clear();



private:

	std::vector<int> prioIndices;
	
	std::vector<RenderQueue::Element> renderQueue;
};
