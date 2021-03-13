#pragma once

#include "Entity.h"
#include "..\Math\AABB.h"

struct InputBuffer;
class RenderQueue;
class EntityManager;

class UIManager
{
public:
	UIManager(EntityManager& entityManager, const InputBuffer& inputBuffer);
	
	void Render(RenderQueue& renderQueue);

private:
	struct UIButton
	{
		UIButton(const Entity& entity, const AABB& box) :
			entity(entity),
			box(box) {};

		Entity entity;
		AABB box;
	};

	bool DoButton(RenderQueue& renderQueue, const UIButton& element);
	void DrawButton(RenderQueue& renderQueue, const UIButton& element, bool isHot, bool isActive);
	Entity hot;
	Entity active;

	std::vector<UIButton> UIButtons;

	const InputBuffer& inputBuffer;
	EntityManager& entityManager;
};