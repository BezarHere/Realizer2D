#pragma once
#include "global.h"

_R2D_NAMESPACE_START_

// nothing was here ;)

enum class PhysicsBodyType : uint8_t
{
	Static, // no velocity or rotation required
	Dynamic, // no rotation required
	Rigid
};

enum CollidorShapeType : uint8_t
{
	Circle,
	Rectangle,
	Segment,
	Convex
};

typedef Rectf AABB;
using sf::Transformable;

class CollidorShape
{
public:
	CollidorShape();

	sf::Vector2f getOffset() const;
	virtual void setOffset(sf::Vector2f offset);

	virtual const Points_t& getPoints() const;
	virtual const Points_t& getNormals() const;
	virtual const AABB& getAABB() const;

	virtual CollidorShapeType getType() const = 0;


protected:

	virtual void updateState() = 0;

protected:
	Vector2 m_offset;
	// no need for those? ignore them ;)
	Points_t m_normals;
	Points_t m_points;
	AABB m_aabb;
};

// idk
class RectangleCollidor : public CollidorShape
{
public:
	RectangleCollidor(Vector2 size);
	RectangleCollidor();

	sf::Vector2f getSize() const;
	virtual void setSize(sf::Vector2f size);

	inline CollidorShapeType getType() const override { return CollidorShapeType::Rectangle; }

protected:
	void updateState() override;

protected:
	sf::Vector2f m_size{ 16.0f, 16.0f };
};

// static circle collidor will be messy
class CircleCollidor : public CollidorShape
{
public:

	real_t getRadius() const;

	const Points_t& getPoints() const override;
	const Points_t& getNormals() const override;
	const AABB& getAABB() const override;

	virtual void setRadius(real_t radius);

	inline CollidorShapeType getType() const override { return CollidorShapeType::Circle; }

protected:
	CircleCollidor();

protected:
	real_t m_radius{ 16.0f };
};

// idk
class ConvexCollidor : public CollidorShape
{
public:
	ConvexCollidor();

	inline CollidorShapeType getType() const override { return CollidorShapeType::Convex; }

protected:
};

// last to be implemented
class SegmentCollidor : public CollidorShape
{
public:

	inline CollidorShapeType getType() const override { return CollidorShapeType::Segment; }

protected:
	SegmentCollidor();
};

_R2D_NAMESPACE_END_