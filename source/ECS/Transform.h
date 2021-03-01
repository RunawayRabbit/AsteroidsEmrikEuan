#pragma once

#include "../Math/Vector2.h"
#include "../Math/Mat3x3.h"

struct Transform
{
	Vector2 pos;

	// Sets the object's scale directly
	void SetScale(float scale);
	void SetScale(float xScale, float yScale);

	void Rotate(float radians);
	void SetRotation(float radians);

private:
	Mat3x3 matrix;
};
