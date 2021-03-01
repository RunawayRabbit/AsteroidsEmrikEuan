#pragma once

#include <math.h>

class Vector2
{
public:

	float x;
	float y;

	// ARITHMETIC OVERLOADS

	inline Vector2 operator*(const float& b) const
	{
		return { x * b, y * b };
	}

	Vector2 operator+(const Vector2& b) const
	{
		Vector2 ret;
		ret.x = x + b.x;
		ret.y = y + b.y;

		return ret;
	}
	inline void operator+=(const Vector2& b)
	{
		x += b.x;
		y += b.y;
	}

	inline Vector2 operator-() const
	{
		Vector2 ret;
		ret.x = -x;
		ret.y = -y;

		return ret;
	}

	Vector2 operator-(const Vector2& b) const
	{
		return (*this) + (-b);
	}

	// Rotation
	inline Vector2 Rotate(const float& radians) const
	{
		Vector2 ret;
		ret.x = (float)(x * cos(radians) - y * sin(radians));
		ret.y = (float)(x * sin(radians) + y * cos(radians));

		return ret;
	}
	inline Vector2 Rot90CCW() const
	{
		return Vector2{ y, -x };
	}
	inline Vector2 Rot90CW() const
	{
		return Vector2{ -y, x };
	}
	inline Vector2 Rot180() const
	{
		return -*this;
	}


	inline static Vector2 zero() { return { 0.0f, 0.0f }; }

};

inline float Dot(Vector2 a, Vector2 b)
{
	return a.x * b.x
		+ a.y * b.y;
}

