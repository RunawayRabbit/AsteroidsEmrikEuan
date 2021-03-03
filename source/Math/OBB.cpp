
#include "AABB.h"
#include "OBB.h"

OBB::OBB(const SDL_Rect& rect, const float& rotation)
{
	this->center = Vector2{ (float)rect.x + (rect.w / 2.0f), (float)rect.y + (rect.h / 2.0f) };
	this->basisX = Vector2{ 1.0f, 0.0f }.Rotate(rotation) * ((float)rect.w / 2.0f);
	this->basisY = Vector2{ 0.0f, 1.0f }.Rotate(rotation) * ((float)rect.h / 2.0f);
}

OBB::OBB(const Vector2& center, const Vector2& ScaledXBasis, const Vector2& scaledYBasis)
{
	this->center = center;
	this->basisX = ScaledXBasis;
	this->basisY = scaledYBasis;
}

OBB::OBB(const Vector2& center, const Vector2& extents, const float& rotation)
{
	this->center = center;
	this->basisX = Vector2{ 1.0f, 0.0f }.Rotate(rotation) * (extents.x);
	this->basisY = Vector2{ 0.0f, 1.0f }.Rotate(rotation) * (extents.y);
}


AABB OBB::Bounds() const
{
	// @TODO: Very likely simd-able
	AABB result = AABB(center, center);
	for (auto& corner : GetCorners())
	{
		result.min.x = corner.x < result.min.x ? corner.x : result.min.x;
		result.min.y = corner.y < result.min.y ? corner.y : result.min.y;
		result.max.x = corner.x > result.max.x ? corner.x : result.max.x;
		result.max.y = corner.y > result.max.y ? corner.y : result.max.y;
	}
	return result;
}

Vector2 OBB::ClosestPointTo(const Vector2& point) const
{
	Vector2 distToCenter = point - this->center;

	// Position in "box space"
	float localX = Dot(distToCenter, this->basisX);
	float localY = Dot(distToCenter, this->basisY);

	// Clamp to the range of -extents to extents
	if (localX < -1.0f)
		localX = -1.0f;
	if (localX > 1.0f)
		localX = 1.0f;

	if (localY < -1.0f)
		localY = -1.0f;
	if (localY > 1.0f)
		localY = 1.0f;

	// Offset by our clamped ranges along X and Y
	return distToCenter + (this->basisX * localX) + (this->basisY * localY);
}

float OBB::DistanceBetweenSq(const Vector2& point) const
{
	Vector2 closestPoint = this->ClosestPointTo(point);
	return Dot(closestPoint - point, closestPoint - point);
}

float OBB::DistanceBetween(const Vector2& point) const
{
	return sqrt(this->DistanceBetweenSq(point));
}

std::array<Vector2, 4> OBB::GetCorners() const
{
	return {
		(center - basisX - basisY),
		(center - basisX + basisY),
		(center + basisX - basisY),
		(center + basisX + basisY),
	};
}