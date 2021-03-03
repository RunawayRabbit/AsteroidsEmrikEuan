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
	SpriteManager(Renderer& renderer, const TransformManager& transManager, const AABB screenAABB, const int capacity);
	SpriteManager() = delete;

	void Create(const Entity entity, const SpriteID spriteID, const RenderQueue::Layer layer, const bool shouldRepeatAtEdges = true);
	void GarbageCollect(const EntityManager& entityManager);

	void Animate(const float& deltaTime);

	void Render(RenderQueue& renderQueue) const;

	void Update();

private:
	const TransformManager& transManager;

	class SpriteCategory
	{
	public:
		SpriteCategory(const TransformManager& transManager);
		SpriteCategory() = delete;

		void AllocateMain(const int newCapacity);
		void AllocateAnimData(const int newCapacity);
		
		void CreateRegular(const Entity entity, const SpriteID spriteID, const SpriteTransform trans);
		void CreateAnimated(const Entity entity, const SpriteID spriteID, const SpriteTransform trans);

		void Update();

		void Render(RenderQueue& renderQueue, const SpriteAtlas& spriteAtlas) const;
		void RenderLooped(RenderQueue& renderQueue, const SpriteAtlas& spriteAtlas, const AABB& screenAABB) const;

		// helper functions
		void DrawAtTop(RenderQueue& renderQueue, const Sprite* sprite, const SpriteTransform* transform, const AABB& screenAABB) const;
		void DrawAtBottom(RenderQueue& renderQueue, const Sprite* sprite, const SpriteTransform* transform, const AABB& screenAABB) const;
		void DrawAtLeft(RenderQueue& renderQueue, const Sprite* sprite, const SpriteTransform* transform, const AABB& screenAABB) const;
		void DrawAtRight(RenderQueue& renderQueue, const Sprite* sprite, const SpriteTransform* transform, const AABB& screenAABB) const;

	private:
		const TransformManager& transManager;

		int size = 0;
		int capacity;

		void* buffer;
		Entity* entities;
		SpriteTransform* transforms;
	};

	AABB screenAABB;
	SpriteAtlas spriteAtlas;

	SpriteCategory repeating;
	SpriteCategory nonRepeating;
};