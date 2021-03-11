#pragma once

#include "..\ECS\Entity.h"
#include "..\Renderer\SpriteAtlas.h"

class TransformManager;

class BackgroundRenderer
{
	BackgroundRenderer(const TransformManager& transformManager);
	void Update();

private:

	const TransformManager& transformManager;

	Entity PlayerShip;

	SpriteID staticBackground;
	SpriteID Parallax1;
	SpriteID Parallax2;
	SpriteID Parallax3;
};