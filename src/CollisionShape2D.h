#pragma once
#include "ObjectComponent2D.h"

_R2D_NAMESPACE_START_

class CollisionShape2D : public ObjectComponent2D
{
public:

	virtual const Points_t& getPoints() const = 0;
	virtual const Points_t& calculateNormals() const = 0;

	inline sf::Vector2f getPosition() const { return m_position; }
	inline float getRotation() const { return m_rotation; }
	inline sf::Vector2f getScale() const { return m_scale; }

	virtual void setPosition(sf::Vector2f value);
	virtual void setRotation(float value);
	virtual void setScale(sf::Vector2f value);


protected:
	sf::Vector2f m_position{ 0.0f, 0.0f };
	float m_rotation{ 0.0f };
	sf::Vector2f m_scale{ 1.0f, 1.0f };
};


class RectangleCollisionShape2D : public CollisionShape2D
{
public:
	const Points_t& getPoints() const override;
	const Points_t& calculateNormals() const override;

private:
	inline static Points_t s_baseNormal{ {1.0f, 0.0f}, {0.0f, 1.0f} };
	sf::Vector2f m_size;
	float m_lastNormalCalcualtionRotation;
};

_R2D_NAMESPACE_END_