
#include "BackgroundRenderer.h";
#include "RenderQueue.h"


BackgroundRenderer::BackgroundRenderer(const TransformManager& transformManager) : 
	transformManager(transformManager)
{

}

void BackgroundRenderer::Render(RenderQueue& renderQueue)
{
	static int offsetX = 0;

	offsetX += 1;
	SDL_Rect offsetTest;
	offsetTest.x = offsetX;
	offsetTest.y = 0;

	renderQueue.Enqueue(SpriteID::PARALLAX_BACKGROUND_3, 0, RenderQueue::Layer::PARALLAX);
	renderQueue.Enqueue(SpriteID::PARALLAX_BACKGROUND_2, 0, RenderQueue::Layer::PARALLAX);
	renderQueue.Enqueue(SpriteID::PARALLAX_BACKGROUND_1, 0, RenderQueue::Layer::PARALLAX);
	renderQueue.Enqueue(SpriteID::STATIC_BACKGROUND, 0, RenderQueue::Layer::BACKGROUND);

}