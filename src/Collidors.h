#pragma once
#include "global.h"

_R2D_NAMESPACE_START_

/*
for any one wondring why this is so bad lookin', without it breaking
its just programer magic and maybe in the future may only god know
how this works.

- Static collidors vs Dynamic collidors:
		there should NOT be and diffrence in behavior only in implmentation!
		any process that works with static col's SHOULD work with dynamic col's or the implmentation is flawd.

- Adding any custom collidors isn't technicly possible coz all collidors are hardcoded in the physics engine

*/

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

	inline sf::Vector2f getOffset() const;
	virtual void setOffset(sf::Vector2f offset);

	virtual const Points_t& getPoints() const;
	virtual const Points_t& getNormals() const;
	virtual const AABB& getAABB() const;

	virtual CollidorShapeType getType() const = 0;

	inline virtual void updateState() {};
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

	void updateState() override;


protected:
	sf::Vector2f m_size{ 16.0f, 16.0f };
};

// static circle collidor will be messy
class CircleCollidor : public CollidorShape
{
public:

	real_t getRadius() const;
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

	inline CollidorShapeType getType() const override { return CollidorShapeType::Convex; }

protected:
	ConvexCollidor();
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