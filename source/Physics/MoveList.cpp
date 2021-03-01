#pragma once

#include "MoveList.h"


MoveList::MoveList()
{

}

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

}