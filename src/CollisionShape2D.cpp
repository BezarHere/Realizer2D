#include "global.h"
#include "CollisionShape2D.h"

_R2D_NAMESPACE_START_

void CollisionShape2D::setPosition(sf::Vector2f value)
{
	m_position = value;
}

void CollisionShape2D::setRotation(float value)
{
	m_rotation = value;
}

void CollisionShape2D::setScale(sf::Vector2f value)
{
	m_scale = value;
}

const Points_t& RectangleCollisionShape2D::getPoints() const
{
	constexpr float STEP = Pi / 2.0f;
	constexpr float OFFSET = Pi / 4.0f;
	return
	{
		sf::Vector2f::polar2coord(OFFSET, 1.0f),
	};
}

const Points_t& RectangleCollisionShape2D::calculateNormals() const
{

	return { s_baseNormal[0].rotated(getRotation()), s_baseNormal[1].rotated(getRotation()) };
}

_R2D_NAMESPACE_END_