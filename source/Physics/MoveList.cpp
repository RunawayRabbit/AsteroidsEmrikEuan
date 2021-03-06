#pragma once

#include "MoveList.h"


MoveList::MoveList(const TransformManager& transformManager) :
	transformManager(transformManager){}

void MoveList::Clear()
{
	data.clear();
}

size_t MoveList::Size() const
{
	return data.size();
}

void MoveList::Enqueue(Entity entity, Vector2 velocity, float angularVelocity)
{
	// @TODO: Rearrange this data to make more sense to the collision detector.

	Entry entry;
	entry.id = entity;
	entry.Velocity = velocity;
	entry.angularVelocity = angularVelocity;
	transformManager.Get(entity, &entry.transform);

	data.push_back(entry);
}