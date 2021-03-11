#pragma once

#include "..\ECS\Entity.h"
#include "..\Renderer\SpriteAtlas.h"

class TransformManager;
class RenderQueue;

class BackgroundRenderer
{
public:
	BackgroundRenderer(const TransformManager& transformManager);
	void Render(RenderQueue& renderQueue);

private:

	const TransformManager& transformManager;

	Entity PlayerShip;

	SpriteID staticBackground;
	SpriteID Parallax1;
	SpriteID Parallax2;
	SpriteID Parallax3;
};