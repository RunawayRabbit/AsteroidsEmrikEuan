
#include <iostream>

#include "SpriteManager.h"

#include "..\ECS\Sprite.h"

#include "..\Renderer\RenderQueue.h"

#include "..\Math\OBB.h"


SpriteManager::SpriteManager(Renderer& renderer, const TransformManager& transManager, const AABB screenAABB, const int capacity) :
	repeating(transManager, capacity),
	nonRepeating(transManager, capacity),
	spriteAtlas(renderer),
	transManager(transManager),
	screenAABB(screenAABB)
{}

void SpriteManager::Animate(const float& deltaTime)
{
	// Trust that our shit is in SORTED order at this point with
	// animated sprites at the front.

	// Loop over sprite instances until we get to the first non-animated one


	// AnimationStates have to be tracked separately in a different array, looks like.
	// OFF TO SPRITECATEGORY WE GO...
}

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

	const Sprite* sprite = spriteAtlas.Get(spriteID);

	SpriteTransform spriteTransform;

	spriteTransform.id = spriteID;

	SDL_Rect transPosition;
	transPosition.x = ((int)trans.pos.x - sprite->source.w / 2);
	transPosition.y = ((int)trans.pos.y - sprite->source.h / 2);
	transPosition.w = sprite->source.w;
	transPosition.h = sprite->source.h;
	spriteTransform.position = transPosition;
	spriteTransform.rotation = trans.rot;
	spriteTransform.layer = layer;

	// Pipe the call to the correct function.
	if (shouldRepeatAtEdges)
	{
		if (SpriteAtlas::isAnimated(spriteID))
			repeating.CreateAnimated(entity, spriteID, spriteTransform);
		else
			repeating.CreateRegular(entity, spriteID, spriteTransform);
	}
	else
	{
		if (SpriteAtlas::isAnimated(spriteID))
			nonRepeating.CreateAnimated(entity, spriteID, spriteTransform);
		else
			nonRepeating.CreateRegular(entity, spriteID, spriteTransform);
	}
}

void SpriteManager::Update()
{
	repeating.Update();
	nonRepeating.Update();
}


void SpriteManager::GarbageCollect(const EntityManager& entityManager)
{
	// @STUB
}






// PULL THIS INTO IT'S OWN CPP AT THIS POINT
// SPRITE CATEGORY

#pragma warning(push)
//#pragma warning(disable: 4705)
SpriteManager::SpriteCategory::SpriteCategory(const TransformManager& transManager, const int capacity) :
	transManager(transManager)
{
	AllocateMain(capacity);
}
#pragma warning(pop)


void SpriteManager::SpriteCategory::AllocateMain(const int newCapacity)
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
		const Sprite* sprite = spriteAtlas.Get(transform->id);

		renderQueue.Enqueue(sprite->texture, sprite->source, transform->position, transform->rotation, transform->layer);
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
		const Sprite* sprite = spriteAtlas.Get(transform->id);
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
				DrawAtBottom(renderQueue, sprite, transform, screenAABB);

				if (spriteAABB.left < screenAABB.left)
				{
					// Case A
					DrawAtRight(renderQueue, sprite, transform, screenAABB);

					// DrawAtBottomRight
					SDL_Rect newPos = transform->position;
					newPos.y += screenAABB.bottom;
					newPos.x += screenAABB.right;
					renderQueue.Enqueue(sprite->texture, sprite->source, newPos, transform->rotation, transform->layer);
				}				
				else if (spriteAABB.right > screenAABB.right)
				{
					// Case C
					DrawAtLeft(renderQueue, sprite, transform, screenAABB);
					
					// DrawAtBottomLeft
					SDL_Rect newPos = transform->position;
					newPos.y += screenAABB.bottom;
					newPos.x -= screenAABB.right;
					renderQueue.Enqueue(sprite->texture, sprite->source, newPos, transform->rotation, transform->layer);
				}
			}
			else if (spriteAABB.bottom > screenAABB.bottom)
			{
				// Case F - G - H
				DrawAtTop(renderQueue, sprite, transform, screenAABB);
				if (spriteAABB.left < screenAABB.left)
				{
					// Case F
					DrawAtRight(renderQueue, sprite, transform, screenAABB);

					// DrawAtTopRight

					SDL_Rect newPos = transform->position;
					newPos.y -= screenAABB.bottom;
					newPos.x += screenAABB.right;
					renderQueue.Enqueue(sprite->texture, sprite->source, newPos, transform->rotation, transform->layer);
				}
				else if (spriteAABB.right > screenAABB.right)
				{
					// Case H
					DrawAtLeft(renderQueue, sprite, transform, screenAABB);

					// DrawAtTopLeft

					SDL_Rect newPos = transform->position;
					newPos.y -= screenAABB.bottom;
					newPos.x -= screenAABB.right;
					renderQueue.Enqueue(sprite->texture, sprite->source, newPos, transform->rotation, transform->layer);

				}
			}
			else
			{
				// Case D - E
				if (spriteAABB.left < screenAABB.left)
				{
					// Case D
					DrawAtRight(renderQueue, sprite, transform, screenAABB);
				}
				else if (spriteAABB.right > screenAABB.right)
				{
					// Case E
					DrawAtLeft(renderQueue, sprite, transform, screenAABB);
				}
			}
		}

		// Render at the original position
		renderQueue.Enqueue(sprite->texture, sprite->source, transform->position, transform->rotation, transform->layer);
	}
}


void SpriteManager::SpriteCategory::CreateRegular(const Entity entity, const SpriteID spriteID, const SpriteTransform trans)
{
	if (size == capacity)
	{
		// We're about to overrun our buffer, we gotta scale.
		AllocateMain((size_t)(size * (size_t)2));
	}
	
	// Insert our data at the back of the data store
	*(entities + size) = entity;
	*(transforms + size) = trans;

	++size;
}

void SpriteManager::SpriteCategory::CreateAnimated(const Entity entity, const SpriteID spriteID, const SpriteTransform trans)
{
	// @STUB
}

void SpriteManager::SpriteCategory::Update()
{
	int i = 0;
	while(i < size)
	{
		Entity* entity = (entities + i);
		SpriteTransform* spriteTrans = (transforms + i);
		
		Transform transform;
		if (transManager.Get(*entity, &transform))
		{
			spriteTrans->rotation = transform.rot;
			spriteTrans->position.x = transform.pos.x - spriteTrans->position.w / 2;
			spriteTrans->position.y = transform.pos.y - spriteTrans->position.h / 2;

			++i;
		}
		else
		{
			// Transform appears to have been deleted.
			// Do the swap to remove it from the list.
			Entity* lastActiveEntity = entities + size - 1;
			SpriteTransform* lastActiveTransform = transforms + size - 1;

			*(entities + i) = *(lastActiveEntity);
			*(transforms + i) = *(lastActiveTransform);

			--size;
		}
	}
}