#include "global.h"
#include "Segment.h"

_R2D_NAMESPACE_START_


Segment::Segment()
	: a{}, b{}
{
}

Segment::Segment(Vector2 start, Vector2 end)
	: a{start}, b{end}
{
}

Segment::operator Vector2() const
{
	return b - a;
}

real_t Segment::length() const
{
		return a.distance(b);
}

real_t Segment::lengthSquared() const
{
		return a.distanceSquared(b);
}

// TODO
bool Segment::intersects(const Segment& other) const
{
	return false;
}

real_t Segment::intersection(const Segment& other) const
{
	return real_t();
}

_R2D_NAMESPACE_END_