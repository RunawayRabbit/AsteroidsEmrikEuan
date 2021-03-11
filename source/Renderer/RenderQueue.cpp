#pragma once

#include <algorithm>

#include "RenderQueue.h"

RenderQueue::RenderQueue() : 
	renderQueue()
{
	renderQueue.reserve(128); // arbitrary, but a decent starting size for total number of rendered sprites?)
}

void RenderQueue::Enqueue(SDL_Texture* texture, const SDL_Rect& sourceRect, const SDL_Rect& targetRect, const float rotation, const RenderQueue::Layer layer)
{
	//@NOTE: I used to do some complex queueing here where sorted insertion became O(log k) where k is the number of layers in use.
	// It turns out that having an O(1) insert and doing the sort at the end is faster, since we enqueue far more often than we
	// fetch.

	Element el;
	el.tex = texture;
	el.srcRect = sourceRect;
	el.dstRect = targetRect;
	el.angle = rotation;
	el.layer = layer;

	renderQueue.push_back(el);
}


#include <iostream>

// @TODO: Consider locking here and unlocking on clear. Rename methods to be explicit if you do so.
const std::vector<RenderQueue::Element>& RenderQueue::GetRenderQueue()
{
	std::sort(renderQueue.begin(), renderQueue.end(),
		[](const RenderQueue::Element& a, const RenderQueue::Element& b) -> bool
		{
			return a.layer > b.layer;
		});

	return renderQueue;
}

void RenderQueue::Clear()
{
	renderQueue.clear();
}