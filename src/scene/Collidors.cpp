#include "global.h"
#include "Collidors.h"

_R2D_NAMESPACE_START_

CollidorShape::CollidorShape()
{
	updateState();
}

inline sf::Vector2f CollidorShape::getOffset() const
{
	return sf::Vector2f();
}

void CollidorShape::setOffset(sf::Vector2f offset)
{
	m_offset = offset;
}


const Points_t& CollidorShape::getPoints() const
{
	return m_points;
}

const Points_t& CollidorShape::getNormals() const
{
	return m_normals;
}

const AABB& CollidorShape::getAABB() const
{
	return m_aabb;
}

real_t CircleCollidor::getRadius() const
{
	return m_radius;
}

void CircleCollidor::setRadius(real_t radius)
{
	m_radius = radius;
}

CircleCollidor::CircleCollidor()
{
}

ConvexCollidor::ConvexCollidor()
{
}

SegmentCollidor::SegmentCollidor()
{
}

sf::Vector2f RectangleCollidor::getSize() const
{
	return m_size;
}

void RectangleCollidor::setSize(sf::Vector2f size)
{
	m_size = size;
	updateState();
}

void RectangleCollidor::updateState()
{
	Vector2 half_size = m_size / 2.0f;

	m_points =
	{
		-half_size + m_offset,
		Vector2(half_size.x, -half_size.y) + m_offset,
		half_size + m_offset,
		Vector2(-half_size.x, half_size.y) + m_offset
	};

	m_normals =
	{
		Vector2(1.0f, 0.0f),
		Vector2(0.0f, 1.0f)
	};
	
	m_aabb = { -half_size, half_size };
}

RectangleCollidor::RectangleCollidor(Vector2 size)
	: m_size(size)
{
	updateState();
}

RectangleCollidor::RectangleCollidor()
{
	updateState();
}

_R2D_NAMESPACE_END_