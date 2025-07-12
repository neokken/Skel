#pragma once



// AABB & Line was based on the one I made for the intake game
// Which has been based on what made in a 3dgep discord event

struct Line
{
    Line(const float2& p0, const float2& p1)
        : p0{ p0 }
        , p1{ p1 }
    {}

    float2 p0{ 0.0f };
    float2 p1{ 0.0f };

    // Shrink the both points of the line towards the center of the line.
    Line shrink(float size) const
    {
        const float2 dir = normalize(p1 - p0);
        return { p0 + dir * size, p1 - dir * size };
    }
};


struct AABB
{	
	AABB(const float2& a, const float2& b)
		: topLeft		{ std::min(a.x, b.x), std::min(a.y, b.y)}
		, bottomRight	{ std::max(a.x, b.x), std::max(a.y, b.y)} {}


    void setPosition(const float2& newPosition);


    float width() const { return bottomRight.x - topLeft.x; }
    float height() const { return bottomRight.y - topLeft.y; }

    // Return the left-most point of the AABB.
    float left() const
    {
        return topLeft.x;
    }

    // Return the right-most point of the AABB.
    float right() const
    {
        return bottomRight.x;
    }

    // Return the top-most point of the AABB.
    float top() const
    {
        return topLeft.y;
    }

    // Return the bottom-most point of the AABB.
    float bottom() const
    {
        return bottomRight.y;
    }

    Line leftEdge() const
    {
        return { {topLeft.x, topLeft.y}, {topLeft.x, bottomRight.y} };
    }

    Line rightEdge() const
    {
        return { {bottomRight.x, topLeft.y}, {bottomRight.x, bottomRight.y} };
    }

    Line topEdge() const
    {
        return { {topLeft.x, topLeft.y}, {bottomRight.x, topLeft.y} };
    }

    Line bottomEdge() const
    {
        return { {topLeft.x, bottomRight.y}, {bottomRight.x, bottomRight.y} };
    }



    // Check if a point is inside the AABB.
    bool contains(const float2 point) const;
    

    // Check if another AABB is intersecting with this one.
    bool intersect(const AABB& other) const;


    // Check to see if the line {p0, p1} intersects the AABB
    bool intersect(const float2& p0, const float2& p1) const;
    
    bool intersect(const Line& line) const;

    bool resolveHorizontalCollisions(const AABB& other, float2& velocity);
    bool resolveVerticalCollisions(const AABB& other, float2& velocity);


    void draw(Surface& target, int2 offset, uint color) const;
    

	float2 topLeft{ 0.f };
	float2 bottomRight{ 0.f };
};