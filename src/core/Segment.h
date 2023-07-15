#pragma once
#include "global.h"

_R2D_NAMESPACE_START_

// a segment that starts at 'a' and ends at 'b'
struct Segment
{
	Segment();
	Segment(Vector2 start, Vector2 end);

	explicit operator Vector2() const;


	real_t length() const;
	real_t lengthSquared() const;

	bool intersects(const Segment& other) const;
	real_t intersection(const Segment& other) const;

	

	Vector2 a, b;
};

_R2D_NAMESPACE_END_