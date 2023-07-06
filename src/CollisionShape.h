#pragma once

_R2D_NAMESPACE_START_

typedef Rectf AABB;
using sf::Transformable;

class CollidorShape : public Transformable
{
public:

	virtual const Points_t& getPoints() const = 0;
	virtual const Points_t& getNormals() const = 0;

	inline sf::Vector2f getPosition() const { return Transformable::getPosition(); }
	inline float getRotation() const { return Transformable::getRotation(); }
	inline sf::Vector2f getScale() const { return Transformable::getScale(); }
	
	virtual void setPosition(sf::Vector2f value);
	virtual void setRotation(float value);
	virtual void setScale(sf::Vector2f value);

	virtual void updateTransform();

	virtual const AABB& getAABB() = 0;

};

// static colldiors are more efficent for static object becuse they cache most things after transformations
// wich is great for unmoving static objects but realy bad for dynamic always-moving objects
class StaticCollidor : public CollidorShape
{
public:
	const Points_t& getPoints() const override;
	const Points_t& getNormals() const override;
	const AABB& getAABB() override;

	void updateTransform() override;

protected:
	virtual void updateState() = 0;
protected:
	Points_t m_normals;
	Points_t m_points;
	AABB m_aabb;
};

class StaticRectangleCollidor : public StaticCollidor
{
public:
	StaticRectangleCollidor();

	sf::Vector2f getSize() const;
	void setSize(sf::Vector2f size);

private:
	void updateState() override;

private:
	inline static Points_t s_baseNormal{ {1.0f, 0.0f}, {0.0f, 1.0f} };
	sf::Vector2f m_size;
};

_R2D_NAMESPACE_END_