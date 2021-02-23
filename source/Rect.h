#pragma once

#include "Vector2.h"

class Rect
{
public:
	Rect(float top, float bottom, float left, float right)
	{
		this->top = top;
		this->bottom = bottom;
		this->left = left;
		this->right = right;
	}

	Rect(Vector2 min, Vector2 max)
	{
		this->min = min;
		this->max = max;
	}

	bool Contains(Vector2 point) const
	{
		return point.x > left && point.x < right&&
			point.y > top && point.y < bottom;
	}



	union
	{
		struct
		{
			Vector2 min;
			Vector2 max;
		};
		struct
		{
			float left;
			float top;
			float right;
			float bottom;
		};
	};
};