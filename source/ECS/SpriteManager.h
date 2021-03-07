#pragma once

#include "Sprite.h"

#include "Entity.h"
#include "EntityManager.h"


#include "..\Math\Vector2.h"
#include "..\Math\AABB.h"

#include "..\Renderer\RenderQueue.h"
#include "..\Renderer\SpriteAtlas.h" // THIS FILE NEEDS TO BE MOVED TO ECS?!?!?

#include "TransformManager.h"

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
	SpriteManager(Renderer& renderer, const TransformManager& transManager, const EntityManager& entityManager, const AABB screenAABB, const int capacity);
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

		void Render(RenderQueue& renderQueue, const SpriteAtlas& spriteAtlas) const;
		void RenderLooped(RenderQueue& renderQueue, const SpriteAtlas& spriteAtlas, const AABB& screenAABB) const;

		// helper functions
		void DrawAtTop(RenderQueue& renderQueue, const Sprite* sprite, const SpriteTransform* transform, const AABB& screenAABB) const;
		void DrawAtBottom(RenderQueue& renderQueue, const Sprite* sprite, const SpriteTransform* transform, const AABB& screenAABB) const;
		void DrawAtLeft(RenderQueue& renderQueue, const Sprite* sprite, const SpriteTransform* transform, const AABB& screenAABB) const;
		void DrawAtRight(RenderQueue& renderQueue, const Sprite* sprite, const SpriteTransform* transform, const AABB& screenAABB) const;

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

	AABB screenAABB;
	SpriteAtlas spriteAtlas;

	SpriteCategory repeating;
	SpriteCategory nonRepeating;
};