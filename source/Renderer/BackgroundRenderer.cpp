
#include "BackgroundRenderer.h";
#include "RenderQueue.h"


BackgroundRenderer::BackgroundRenderer(const TransformManager& transformManager) : 
	transformManager(transformManager)
{

}

void BackgroundRenderer::Render(RenderQueue& renderQueue)
{
	renderQueue.Enqueue(SpriteID::STATIC_BACKGROUND, 0, RenderQueue::Layer::BACKGROUND);
}