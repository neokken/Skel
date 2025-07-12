#include "precomp.h"
#include "aabb.h"


void AABB::setPosition(const float2& newPosition)
{
	float2 diff = newPosition - topLeft;
	topLeft += diff;
	bottomRight += diff;

}

bool AABB::contains(const float2 point) const
{
	return (point.x >= topLeft.x && point.x <= bottomRight.x) &&
		(point.y >= topLeft.y && point.y <= bottomRight.y);
}

bool AABB::intersect(const AABB& other) const
{
	return (topLeft.x <= other.bottomRight.x && topLeft.y <= other.bottomRight.y) &&
		(bottomRight.x >= other.topLeft.x && bottomRight.y >= other.topLeft.y);
}

bool AABB::intersect(const float2& p0, const float2& p1) const
{
	// Find min and max X for the segment
	float minX = p0.x;
	float maxX = p1.x;

	if (p0.x > p1.x)
	{
		minX = p1.x;
		maxX = p0.x;
	}

	// Find the intersection of the segment's and rectangle's x-projections
	if (maxX > bottomRight.x)
	{
		maxX = bottomRight.x;
	}

	if (minX < topLeft.x)
	{
		minX = topLeft.x;
	}

	if (minX > maxX) // If their projections do not intersect return false
	{
		return false;
	}

	// Find corresponding min and max Y for min and max X we found before
	float minY = p0.y;
	float maxY = p1.y;

	float dx = p1.x - p0.x;

	if (std::abs(dx) > 0.0000001f)
	{
		float a = (p1.y - p0.y) / dx;
		float b = p0.y - a * p0.x;
		minY = a * minX + b;
		maxY = a * maxX + b;
	}

	if (minY > maxY)
	{
		std::swap(minY, maxY);
	}

	// Find the intersection of the segment's and rectangle's y-projections
	if (maxY > bottomRight.y)
	{
		maxY = bottomRight.y;
	}

	if (minY < topLeft.y)
	{
		minY = topLeft.y;
	}

	if (minY > maxY) // If Y-projections do not intersect return false
	{
		return false;
	}

	return true;
}

bool AABB::intersect(const Line& line) const
{
	return intersect(line.p0, line.p1);
}

bool AABB::resolveVerticalCollisions(const AABB& other, float2& velocity)
{
	// https://github.com/jpvanoosten/tmpl8/blob/Physics_Test
	// Made in the discord events
	// Changed so it works in new template, also now it just changes this aabb instead of a parameter of this function


	if (velocity.y > 0.f) // Moving down...
	{
		if (intersect(other.topEdge().shrink(1)))
		{
			// Compute intersection.
			const float overlap = other.topLeft.y - bottomRight.y;
			setPosition(topLeft + float2(0.f, overlap));


			velocity.y = 0.0f;

			return true;
		}
	}
	else if (velocity.y < 0.f) // Moving up
	{
		if (intersect(other.bottomEdge().shrink(1)))
		{
			// Compute intersection.
			const float overlap = other.bottomRight.y - topLeft.y;
			setPosition(topLeft + float2(0.f, overlap));

			velocity.y = 0.f;
			return true;
		}
	}

	return false;
}

bool AABB::resolveHorizontalCollisions(const AABB& other, float2& velocity)
{
	// https://github.com/jpvanoosten/tmpl8/blob/Physics_Test
	// Made in the discord events
	// Changed so it works in new template, also now it just changes this aabb instead of a parameter of this function

	if (velocity.x > 0.f) // Moving Right...
	{
		if (intersect(other.leftEdge().shrink(1)))
		{
			// Compute intersection.
			const float overlap = other.topLeft.x - bottomRight.x;
			setPosition(topLeft+float2(overlap,0.f));

			velocity.x = 0.0f;
			return true;
		}
	}
	else if (velocity.x < 0.f) // Moving left...
	{
		if (intersect(other.rightEdge().shrink(1)))
		{
			// Compute intersection.
			const float overlap = other.bottomRight.x - topLeft.x;
			setPosition(topLeft + float2(overlap, 0.f));
			velocity.x = 0.f;
			return true;
		}
	}

	return false;
}




void AABB::draw(Surface& target, int2 offset, uint color) const
{
	target.Box(static_cast<int>(topLeft.x) + offset.x, static_cast<int>(topLeft.y) + offset.y, static_cast<int>(bottomRight.x) + offset.x, static_cast<int>(bottomRight.y) + offset.y, color);
}
