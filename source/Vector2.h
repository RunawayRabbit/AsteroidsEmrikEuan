#pragma once

class Vector2
{
public:

	float x;
	float y;
	Vector2 operator*(const float& b)
	{
		return { x * b, y * b };
	}

	void operator+=(const Vector2& b)
	{
		x += b.x;
		y += b.y;
	}

	static Vector2 zero() { return { 0.0f, 0.0f }; }

};

