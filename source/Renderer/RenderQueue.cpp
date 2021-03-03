#pragma once

#include "RenderQueue.h"

RenderQueue::RenderQueue() : 
	renderQueue(),
	prioIndices((int)RenderQueue::Layer::COUNT)
{
	renderQueue.reserve(64); // arbitrary, but a decent starting size for total number of rendered sprites?)
}

void RenderQueue::Enqueue(SDL_Texture* texture, const SDL_Rect& sourceRect, const SDL_Rect& targetRect, const float rotation, const RenderQueue::Layer layer)
{
	//@TODO: Is it more efficient to just store the layer in Element and sort the vector
	// when someone requests the renderQueue? Depends on how the list ends up being used
	// I think?

	Element el;
	el.tex = texture;
	el.srcRect = sourceRect;
	el.dstRect = targetRect;
	el.angle = rotation;

	renderQueue.push_back(el);

	// Swap the new element into it's correct position. Start at the back, work your way to the front.
	// Stop when we are on the correct layer so we don't increment indices we shouldn't.
	for (int currentLayer = (int)Layer::COUNT-1,
		currentIndex = prioIndices[(int)Layer::COUNT - 1];
		currentLayer > (int)layer;
		--currentLayer)
	{
		// Identify the spot to swap into, increment.
		int swapIndex = prioIndices[currentLayer]++;
		std::swap(renderQueue[currentIndex], renderQueue[swapIndex]);
		currentIndex = swapIndex;
	}
}


// @TODO: Consider locking here and unlocking on clear. Rename methods to be explicit if you do so.
const std::vector<RenderQueue::Element>& RenderQueue::GetRenderQueue() const
{
	return renderQueue;
}

void RenderQueue::Clear()
{
	renderQueue.clear();
	for (auto& index : prioIndices)
		index = 0;
}