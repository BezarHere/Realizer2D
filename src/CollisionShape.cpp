#include "global.h"
#include "CollisionShape.h"

_R2D_NAMESPACE_START_

void CollidorShape::setPosition(sf::Vector2f value)
{
	Transformable::setPosition(value);
	updateTransform();
}

void CollidorShape::setRotation(float value)
{
	Transformable::setRotation(value);
	updateTransform();
}

void CollidorShape::setScale(sf::Vector2f value)
{
	Transformable::setScale(value);
	updateTransform();
}

void CollidorShape::updateTransform()
{

}


const Points_t& StaticCollidor::getPoints() const
{
	return m_points;
}

const Points_t& StaticCollidor::getNormals() const
{
	return m_normals;
}

const AABB& StaticCollidor::getAABB()
{
	return m_aabb;
}

void StaticCollidor::updateTransform()
{
	updateState();
}

StaticRectangleCollidor::StaticRectangleCollidor()
{
	m_normals.resize(2);
	m_points.resize(4);
}


sf::Vector2f StaticRectangleCollidor::getSize() const
{
	return m_size;
}

void StaticRectangleCollidor::setSize(sf::Vector2f size)
{
	m_size = size;
	updateState();
}



void StaticRectangleCollidor::updateState()
{
	const sf::Transform &transform = getTransform();
	sf::Vector2f half_size = m_size / 2.0f;

	m_points =
	{
		transform.transformPoint(-half_size),
		transform.transformPoint(sf::Vector2f(half_size.x, -half_size.y)),
		transform.transformPoint(half_size),
		transform.transformPoint(sf::Vector2f(-half_size.x, half_size.y))
	};

	m_normals =
	{
		transform.transformPoint(1.0f, 0.0f),
		transform.transformPoint(0.0f, 1.0f)
	};
}

_R2D_NAMESPACE_END_