#pragma once

#include <algorithm>

#include "RenderQueue.h"
#include "SpriteID.h"
#include "Sprite.h"


RenderQueue::RenderQueue(Renderer& renderer, int screenWidth, int screenHeight) :
	spriteAtlas(renderer),
	screenWidth(screenWidth),
	screenHeight(screenHeight)
{
	renderQueue.reserve(128); // arbitrary, but a decent starting size for total number of rendered sprites?)
}


void RenderQueue::Enqueue(SpriteID spriteID, const SDL_Rect& targetRect, const float rotation, const RenderQueue::Layer layer)
{
	//@NOTE: I used to do some complex queueing here where sorted insertion became O(log k) where k is the number of layers in use.
	// It turns out that having an O(1) insert and doing the sort at the end is faster, since we enqueue far more often than we
	// fetch.
	Sprite sprite = spriteAtlas.Get(spriteID);

	Element el;
	el.tex = sprite.texture;
	el.srcRect = sprite.source;
	el.dstRect = targetRect;
	el.angle = rotation;
	el.layer = layer;

	renderQueue.push_back(el);
}

void RenderQueue::Enqueue(SpriteID spriteID, const float rotation, const RenderQueue::Layer layer)
{
	SDL_Rect targetRect;
	targetRect.w = screenWidth;
	targetRect.h = screenHeight;
	targetRect.x = 0;
	targetRect.y = 0;
	Enqueue(spriteID, targetRect, rotation, layer);
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