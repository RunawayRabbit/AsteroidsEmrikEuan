#pragma once

#include "Entity.h"
#include "EntityManager.h"

#include "..\Math\Vector2.h"

#include "..\Renderer\RenderQueue.h"
#include "..\Renderer\SpriteAtlas.h"

#include "TransformManager.h"

class AABB;

struct SpriteTransform
{
	SpriteID id;
	SDL_Rect position;
	float rotation;
	RenderQueue::Layer layer;
};

class SpriteManager
{
public:
	SpriteManager(const TransformManager& transManager, const EntityManager& entityManager, const SpriteAtlas& spriteAtlas, const int capacity);
	SpriteManager() = delete;

	void Create(const Entity entity, const SpriteID spriteID, const RenderQueue::Layer layer, const bool shouldRepeatAtEdges = true);

	void Render(RenderQueue& renderQueue) const;

	void Update(const float deltaTime);

private:
	const TransformManager& transManager;

	class SpriteCategory
	{
	public:
		SpriteCategory(const TransformManager& transManager, const EntityManager& entityManager, const int capacity);
		SpriteCategory() = delete;

		void Allocate(const int newCapacity);
		
		void Create(const Entity entity, const SpriteID spriteID, const SpriteTransform trans);

		void Update(const SpriteAtlas& spriteAtlas, const float deltaTime);

		void Render(RenderQueue& renderQueue) const;
		void RenderLooped(RenderQueue& renderQueue, const int screenWidth, const int screenHeight) const;

		// helper functions
		void DrawAtTop(RenderQueue& renderQueue, const SpriteID spriteID, const SpriteTransform* transform, const AABB& screenAABB) const;
		void DrawAtBottom(RenderQueue& renderQueue, const SpriteID spriteID, const SpriteTransform* transform, const AABB& screenAABB) const;
		void DrawAtLeft(RenderQueue& renderQueue, const SpriteID spriteID, const SpriteTransform* transform, const AABB& screenAABB) const;
		void DrawAtRight(RenderQueue& renderQueue, const SpriteID spriteID, const SpriteTransform* transform, const AABB& screenAABB) const;

	private:
		const TransformManager& transManager;
		const EntityManager& entityManager;

		int size = 0;
		int animatedSize = 0;
		int capacity;

		void* buffer;
		Entity* entities;
		SpriteTransform* transforms;

		std::vector<float> currentFrameTimes;
	};

	const SpriteAtlas& spriteAtlas;

	SpriteCategory repeating;
	SpriteCategory nonRepeating;
};