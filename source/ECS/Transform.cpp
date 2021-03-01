
#include "Transform.h"

void Transform::SetScale(float scale)
{
	//upVectorWithScale = upVectorWithScale.Normalized() * scale;
}

void Transform::SetScale(float xScale, float yScale)
{
	//upVectorWithScale = upVectorWithScale.Normalized() * V2 { xScale, yScale };
}

void Transform::Rotate(float radians)
{
	//upVectorWithScale = upVectorWithScale.Rotate(radians);
}

void Transform::SetRotation(float radians)
{
	//upVectorWithScale = V2::FromAngle(radians);
}