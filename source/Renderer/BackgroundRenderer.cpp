
#include "BackgroundRenderer.h"
#include "RenderQueue.h"
#include "..\ECS\SpriteManager.h"

BackgroundRenderer::BackgroundRenderer(const TransformManager& transformManager, const AABB& screen) :
	transformManager(transformManager),
	playerShip(Entity::null()),
	screen(screen),
	position(0.0f, 0.0f)
{
	SDL_Rect screenRect;
	screenRect.x = 0;
	screenRect.y = 0;
	screenRect.w = screen.max.x;
	screenRect.h = screen.max.y;

	// Background
	background.id = SpriteID::STATIC_BACKGROUND;
	background.layer = RenderQueue::Layer::BACKGROUND;
	background.rotation = 0;
	background.position = screenRect;

	// Parallax 1
	parallax1.id = SpriteID::PARALLAX_BACKGROUND_1;
	parallax1.layer = RenderQueue::Layer::PARALLAX;
	parallax1.rotation = 0;
	parallax1.position = screenRect;

	// Parallax 2
	parallax2.id = SpriteID::PARALLAX_BACKGROUND_2;
	parallax2.layer = RenderQueue::Layer::PARALLAX;
	parallax2.rotation = 0;
	parallax2.position = screenRect;

	// Parallax 3
	parallax3.id = SpriteID::PARALLAX_BACKGROUND_3;
	parallax3.layer = RenderQueue::Layer::PARALLAX;
	parallax3.rotation = 0;
	parallax3.position = screenRect;
}

void BackgroundRenderer::Render(RenderQueue& renderQueue, const float& deltaTime)
{
	// Static background
	renderQueue.Enqueue(SpriteID::STATIC_BACKGROUND, 0, RenderQueue::Layer::BACKGROUND);

	position.x += 20.0f * deltaTime;
	position.y += 10.0f * deltaTime;
	
	if (playerShip == Entity::null())
	{
		parallax1.position.x = floor(position.x);
		if (parallax1.position.x < 0)
			parallax1.position.x += screen.max.x;
		else if (parallax1.position.x > screen.max.x)
			parallax1.position.x -= screen.max.x;

		parallax1.position.y = floor(position.y);
		if (parallax1.position.y < 0)
			parallax1.position.y += screen.max.y;
		else if (parallax1.position.y > screen.max.y)
			parallax1.position.y -= screen.max.y;



		parallax2.position.x = floor(position.x / 2);
		if (parallax2.position.x < 0)
			parallax2.position.x += screen.max.x;
		else if (parallax2.position.x > screen.max.x)
			parallax2.position.x -= screen.max.x;

		parallax2.position.y = floor(position.y / 2);
		if (parallax2.position.y < 0)
			parallax2.position.y += screen.max.y;
		else if (parallax2.position.y > screen.max.y)
			parallax2.position.y -= screen.max.y;



		parallax3.position.x = floor(position.x / 4);
		if (parallax3.position.x < 0)
			parallax3.position.x += screen.max.x;
		else if (parallax3.position.x > screen.max.x)
			parallax3.position.x -= screen.max.x;

		parallax3.position.y = floor(position.y / 4);
		if (parallax3.position.y < 0)
			parallax3.position.y += screen.max.y;
		else if (parallax3.position.y > screen.max.y)
			parallax3.position.y -= screen.max.y;



		background.position.x = floor(position.x / 5);
		if (background.position.x < 0)
			background.position.x += screen.max.x;
		else if (background.position.x > screen.max.x)
			background.position.x -= screen.max.x;

		background.position.y = floor(position.y / 5);
		if (background.position.y < 0)
			background.position.y += screen.max.y;
		else if (background.position.y > screen.max.y)
			background.position.y -= screen.max.y;
	}

	renderQueue.EnqueueLooped(background);
	renderQueue.EnqueueLooped(parallax1);
	renderQueue.EnqueueLooped(parallax2);
	renderQueue.EnqueueLooped(parallax3);
}