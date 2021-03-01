#pragma once

#include <array>
#include <sstream>

#include "Vector2.h"

struct Mat3x3
{
	union
	{
		struct
		{
			float m11;
			float m12;
			float m13;
			float m21;
			float m22;
			float m23;
			float m31;
			float m32;
			float m33;
		};
		struct
		{
			float row1[3];
			float row2[3];
			float row3[3];
		};
		float e[9];
	};

	// from 0 to 2
	inline std::array<float, 3> GetColumn(int column);

	static inline Mat3x3 Identity();

	inline Mat3x3 operator*(float scale);
	inline void operator*=(float scale);

	inline Mat3x3 operator*(Mat3x3 b);

	inline Vector2 operator*(const Vector2 vec);

	static inline Mat3x3 Translation(const Vector2 translation);
	static inline Mat3x3 Rotation(float radians);
	static inline Mat3x3 Scale(float scale);


	inline void Translate(const Vector2 translation);
	inline void Rotate(float radians);

	inline std::string ToString() const;

};

std::array<float, 3> Mat3x3::GetColumn(int column)
{
	return { e[column], e[column + 3], e[column + 6] };
}

Mat3x3 Mat3x3::operator*(float scale)
{
	Mat3x3 ret = *this;

	ret.m11 *= scale;
	ret.m12 *= scale;
	ret.m13 *= scale;
	ret.m21 *= scale;
	ret.m22 *= scale;
	ret.m23 *= scale;
	ret.m31 *= scale;
	ret.m32 *= scale;
	ret.m33 *= scale;

	return ret;
}

void Mat3x3::operator*=(float scale)
{
	m11 *= scale;
	m12 *= scale;
	m13 *= scale;
	m21 *= scale;
	m22 *= scale;
	m23 *= scale;
	m31 *= scale;
	m32 *= scale;
	m33 *= scale;
}

Vector2 Mat3x3::operator*(const Vector2 vec)
{
	Vector2 res;
	res.x = (vec.x * m11) + (vec.y * m12) + m13;
	res.y = (vec.x * m21) + (vec.y * m22) + m23;

	return res;
}

Mat3x3 Mat3x3::Identity()
{
	return
	{ 1, 0, 0,
	0, 1, 0,
	0, 0, 1 };
}

Mat3x3 Mat3x3::Rotation(float radians)
{
	Mat3x3 ret = Identity();

	float cosRad = cos(radians);
	float sinRad = sin(radians);

	ret.m11 = cosRad;
	ret.m22 = cosRad;
	ret.m12 = -sinRad;
	ret.m21 = sinRad;

	return ret;
}

Mat3x3 Mat3x3::Translation(const Vector2 translation)
{
	Mat3x3 ret = Identity();

	ret.m13 = translation.x;
	ret.m23 = translation.y;

	return ret;
}

Mat3x3 Mat3x3::Scale(float scale)
{
	return Identity() * scale;
}


Mat3x3 Mat3x3::operator*(Mat3x3 b)
{
	Mat3x3 ret{};

	ret.m11 = (this->m11 * b.m11) + (this->m12 * b.m21) + (this->m13 * b.m31);
	ret.m12 = (this->m11 * b.m12) + (this->m12 * b.m22) + (this->m13 * b.m32);
	ret.m13 = (this->m11 * b.m13) + (this->m12 * b.m23) + (this->m13 * b.m33);

	ret.m21 = (this->m21 * b.m11) + (this->m22 * b.m21) + (this->m23 * b.m31);
	ret.m22 = (this->m21 * b.m12) + (this->m22 * b.m22) + (this->m23 * b.m32);
	ret.m23 = (this->m21 * b.m13) + (this->m22 * b.m23) + (this->m23 * b.m33);

	ret.m31 = (this->m31 * b.m11) + (this->m32 * b.m21) + (this->m33 * b.m31);
	ret.m32 = (this->m31 * b.m12) + (this->m32 * b.m22) + (this->m33 * b.m32);
	ret.m33 = (this->m31 * b.m13) + (this->m32 * b.m23) + (this->m33 * b.m33);

	return ret;
}

std::string Mat3x3::ToString() const
{
	std::ostringstream s;
	for (size_t i = 0; i < 3; i++)
	{
		for (size_t j = 0; j < 3; j++)
		{
			s << std::to_string(e[3 * i + j]) << " ";
		}
		if (i < 2)
			s << std::endl;
	}

	return s.str();
}

void Mat3x3::Translate(Vector2 translation)
{
	*this = *this * Translation(translation);
}

void Mat3x3::Rotate(float radians)
{
	*this = Rotation(radians) * *this;
}

