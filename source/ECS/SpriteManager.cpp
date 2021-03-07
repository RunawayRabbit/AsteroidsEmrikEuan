
#include <iostream>

#include "SpriteManager.h"

#include "..\ECS\Sprite.h"

#include "..\Renderer\RenderQueue.h"

#include "..\Math\OBB.h"


SpriteManager::SpriteManager(Renderer& renderer, const TransformManager& transManager, const EntityManager& entityManager, const AABB screenAABB, const int capacity) :
	spriteAtlas(renderer), 
	repeating(transManager, entityManager, capacity),
	nonRepeating(transManager, entityManager, capacity),
	transManager(transManager),
	screenAABB(screenAABB)
{}

void SpriteManager::Render(RenderQueue& renderQueue) const
{
	repeating.RenderLooped(renderQueue, spriteAtlas, screenAABB);
	nonRepeating.Render(renderQueue, spriteAtlas);
}


void SpriteManager::Create(const Entity entity, const SpriteID spriteID, const RenderQueue::Layer layer, const bool shouldRepeatAtEdges)
{
	//@TODO: These APIs aren't following the same conventions...
	
	// Get the date we need to work with.
	Transform trans;
	transManager.Get(entity, &trans);

	const Sprite sprite = spriteAtlas.Get(spriteID);

	SpriteTransform spriteTransform;

	spriteTransform.id = spriteID;

	SDL_Rect transPosition;
	transPosition.x = ((int)trans.pos.x - sprite.source.w / 2);
	transPosition.y = ((int)trans.pos.y - sprite.source.h / 2);
	transPosition.w = sprite.source.w;
	transPosition.h = sprite.source.h;
	spriteTransform.position = transPosition;
	spriteTransform.rotation = trans.rot;
	spriteTransform.layer = layer;

	// Pipe the call to the correct function.
	if (shouldRepeatAtEdges)
	{
		repeating.Create(entity, spriteID, spriteTransform);
	}
	else
	{
		nonRepeating.Create(entity, spriteID, spriteTransform);
	}
}

void SpriteManager::Update(const float deltaTime)
{
	repeating.Update(spriteAtlas, deltaTime);
	nonRepeating.Update(spriteAtlas, deltaTime);
}





// SPRITE CATEGORY

#pragma warning(push)
SpriteManager::SpriteCategory::SpriteCategory(const TransformManager& transManager, const EntityManager& entityManager, const int capacity) :
	transManager(transManager),
	entityManager(entityManager)
{
	Allocate(capacity);
}
#pragma warning(pop)


void SpriteManager::SpriteCategory::Allocate(const int newCapacity)
{
	capacity = newCapacity;

	// Allocate new memory
	const size_t elementSizeInBytes = sizeof(Entity) + sizeof(SpriteTransform);
	void* newBuffer = new size_t[(elementSizeInBytes * newCapacity)];

	// Set up new pointers for where our data will go
	Entity* newEntities = (Entity*)newBuffer;
	SpriteTransform* newTransforms = (SpriteTransform*)(newEntities + newCapacity);

	if (size > 0)
	{
		// Copy the data to the new buffer
		memcpy(newEntities, entities, sizeof(Entity) * size);
		memcpy(newTransforms, transforms, sizeof(SpriteTransform) * size);
	}

	// Switch the pointers around
	entities = newEntities;
	transforms = newTransforms;

	// Switch the buffers and free the old memory
	delete buffer;
	buffer = newBuffer;	
}

void SpriteManager::SpriteCategory::Render(RenderQueue& renderQueue, const SpriteAtlas& spriteAtlas) const
{
	for (auto i = 0; i < size; i++)
	{
		const SpriteTransform* transform = transforms + i;
		const Sprite sprite = spriteAtlas.Get(transform->id);

		renderQueue.Enqueue(sprite.texture, sprite.source, transform->position, transform->rotation, transform->layer);
	}
}

void SpriteManager::SpriteCategory::DrawAtTop(RenderQueue &renderQueue, const Sprite* sprite, const SpriteTransform* transform, const AABB& screenAABB) const
{
	SDL_Rect newPos = transform->position;
	newPos.y -= screenAABB.bottom;
	renderQueue.Enqueue(sprite->texture, sprite->source, newPos, transform->rotation, transform->layer);

}
void SpriteManager::SpriteCategory::DrawAtBottom(RenderQueue& renderQueue, const Sprite* sprite, const SpriteTransform* transform, const AABB& screenAABB) const
{
	SDL_Rect newPos = transform->position;
	newPos.y += screenAABB.bottom;
	renderQueue.Enqueue(sprite->texture, sprite->source, newPos, transform->rotation, transform->layer);
}

void SpriteManager::SpriteCategory::DrawAtLeft(RenderQueue& renderQueue, const Sprite* sprite, const SpriteTransform* transform, const AABB& screenAABB) const
{
	SDL_Rect newPos = transform->position;
	newPos.x -= screenAABB.right;
	renderQueue.Enqueue(sprite->texture, sprite->source, newPos, transform->rotation, transform->layer);
}
void SpriteManager::SpriteCategory::DrawAtRight(RenderQueue& renderQueue, const Sprite* sprite, const SpriteTransform* transform, const AABB& screenAABB) const
{
	SDL_Rect newPos = transform->position;
	newPos.x += screenAABB.right;
	renderQueue.Enqueue(sprite->texture, sprite->source, newPos, transform->rotation, transform->layer);
}

void SpriteManager::SpriteCategory::RenderLooped(RenderQueue& renderQueue, const SpriteAtlas& spriteAtlas, const AABB& screenAABB) const
{
	for (auto i = 0; i < size; i++)
	{
		const SpriteTransform* transform = transforms + i;
		const Sprite sprite = spriteAtlas.Get(transform->id);
		OBB spriteOBB = OBB(transform->position, transform->rotation);

		if (!screenAABB.FullyContains(spriteOBB))
		{
			// Sprite is visible on the opposite side. Determine which!
			AABB spriteAABB = spriteOBB.Bounds();

			/*
				PRETTY PICTURE TO VISUALIZE CASES
				  A |  B | C
				____|____|____
				  D |    | E
				____|____|____
				  F	|  G | H
					|    |
			*/

			if (spriteAABB.top < screenAABB.top)
			{
				// Case A - B - C
				DrawAtBottom(renderQueue, &sprite, transform, screenAABB);

				if (spriteAABB.left < screenAABB.left)
				{
					// Case A
					DrawAtRight(renderQueue, &sprite, transform, screenAABB);

					// DrawAtBottomRight
					SDL_Rect newPos = transform->position;
					newPos.y += screenAABB.bottom;
					newPos.x += screenAABB.right;
					renderQueue.Enqueue(sprite.texture, sprite.source, newPos, transform->rotation, transform->layer);
				}				
				else if (spriteAABB.right > screenAABB.right)
				{
					// Case C
					DrawAtLeft(renderQueue, &sprite, transform, screenAABB);
					
					// DrawAtBottomLeft
					SDL_Rect newPos = transform->position;
					newPos.y += screenAABB.bottom;
					newPos.x -= screenAABB.right;
					renderQueue.Enqueue(sprite.texture, sprite.source, newPos, transform->rotation, transform->layer);
				}
			}
			else if (spriteAABB.bottom > screenAABB.bottom)
			{
				// Case F - G - H
				DrawAtTop(renderQueue, &sprite, transform, screenAABB);
				if (spriteAABB.left < screenAABB.left)
				{
					// Case F
					DrawAtRight(renderQueue, &sprite, transform, screenAABB);

					// DrawAtTopRight

					SDL_Rect newPos = transform->position;
					newPos.y -= screenAABB.bottom;
					newPos.x += screenAABB.right;
					renderQueue.Enqueue(sprite.texture, sprite.source, newPos, transform->rotation, transform->layer);
				}
				else if (spriteAABB.right > screenAABB.right)
				{
					// Case H
					DrawAtLeft(renderQueue, &sprite, transform, screenAABB);

					// DrawAtTopLeft

					SDL_Rect newPos = transform->position;
					newPos.y -= screenAABB.bottom;
					newPos.x -= screenAABB.right;
					renderQueue.Enqueue(sprite.texture, sprite.source, newPos, transform->rotation, transform->layer);

				}
			}
			else
			{
				// Case D - E
				if (spriteAABB.left < screenAABB.left)
				{
					// Case D
					DrawAtRight(renderQueue, &sprite, transform, screenAABB);
				}
				else if (spriteAABB.right > screenAABB.right)
				{
					// Case E
					DrawAtLeft(renderQueue, &sprite, transform, screenAABB);
				}
			}
		}

		// Render at the original position
		renderQueue.Enqueue(sprite.texture, sprite.source, transform->position, transform->rotation, transform->layer);
	}
}


void SpriteManager::SpriteCategory::Create(const Entity entity, const SpriteID spriteID, const SpriteTransform trans)
{
	if (size == capacity)
	{
		// We're about to overrun our buffer, we gotta scale.
		Allocate((size_t)(size * 2));
	}
	
	// Insert our data at the back of the data store
	*(entities + size) = entity;
	*(transforms + size) = trans;

	if (SpriteAtlas::isAnimated(spriteID))
	{
		// @TODO: This isn't the most efficient algorithm but I'm assuming the compiler will fix it..?
		std::swap(*(entities + size), *(entities + currentFrameTimes.size()));
		std::swap(*(transforms + size), *(transforms + currentFrameTimes.size()));

		currentFrameTimes.push_back(SpriteAnimationData::frameTime[(int)spriteID]);
	}

	++size;
}

void SpriteManager::SpriteCategory::Update(const SpriteAtlas& spriteAtlas, const float deltaTime)
{
	int i = 0;
	while(i < size)
	{
		Entity* entity = (entities + i);
		SpriteTransform* spriteTrans = (transforms + i);

		Transform transform;
		if (entityManager.Exists(*entity) && transManager.Get(*entity, &transform))
		{
			if (SpriteAtlas::isAnimated(spriteTrans->id))
			{
				currentFrameTimes[i] -= deltaTime;
				if (currentFrameTimes[i] < 0.0f)
				{
					spriteTrans->id = SpriteAnimationData::nextFrameIndex[(int)spriteTrans->id];
					currentFrameTimes[i] += SpriteAnimationData::frameTime[(int)spriteTrans->id];

					const Sprite newSprite = spriteAtlas.Get(spriteTrans->id);
					spriteTrans->position.w = newSprite.source.w;
					spriteTrans->position.h = newSprite.source.h;
				}
			}

			spriteTrans->rotation = transform.rot;
			spriteTrans->position.x = transform.pos.x - spriteTrans->position.w / 2;
			spriteTrans->position.y = transform.pos.y - spriteTrans->position.h / 2;

			++i;
		}
		else
		{
			// Entity or Transform appears to have been deleted.
			// Do the swap to remove it from the list.

			Entity* lastEntity = entities + size - 1;
			SpriteTransform* lastTransform = transforms + size - 1;

			int swapTarget = i;

			if (SpriteAtlas::isAnimated(spriteTrans->id))
			{
				// Maintain sorted order for animated sprites
				swapTarget = currentFrameTimes.size() - 1;

				Entity* lastAnimatedEntity = entities + swapTarget;
				SpriteTransform* lastAnimatedTransform = transforms + swapTarget;

				*(entities + i) = *(lastAnimatedEntity);
				*(transforms + i) = *(lastAnimatedTransform);

				currentFrameTimes[i] = currentFrameTimes.back();
				currentFrameTimes.pop_back();
			}

			*(entities + swapTarget) = *(lastEntity);
			*(transforms + swapTarget) = *(lastTransform);

			--size;
		}
	}
}