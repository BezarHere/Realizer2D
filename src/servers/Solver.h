#pragma once
#include "global.h"
#include "scene/Collidors.h"
#include "components/Physics.h"

_R2D_NAMESPACE_START_

struct CollisionCallback
{
	Vector2 contact{ 0.0f, 0.0f };
	Vector2 pentration{}; // body A will be offseted by pentration / 2.0f and Body B by -pentration / 2.0f
	bool collision{ false };

	void invalidate();

	operator bool() const;
	bool operator!() const;

};

class PhysicsBodyForm
{
public:
	PhysicsBodyForm(components::PhysicsBody* body, const CollidorShape* shape, const Transform2D& transform);

	void tryGenerate();

	const components::PhysicsBody *getBody() const;
	const CollidorShape *getShape() const;
	const Transform2D &getTransform() const;
	const Points_t &getTransformedPoints() const;
	const Points_t &getTransformedNormals() const;
	Vector2 getCenter() const;

	void setApplyedTransform(const Transform2D &transform);

public:
	Vector2 applyedForce{};


private:
	void generate();
private:
	const components::PhysicsBody* m_body;
	const CollidorShape* m_shape;
	const Transform2D &m_transform;
	Points_t m_transformedPoints;
	Points_t m_transformedNormals;
	Vector2 m_center;
	bool m_generated{ false };
};

class Solver
{
	friend class PhysicsServer;
public:
	typedef void(*SolverFunction)
		(PhysicsBodyForm&, PhysicsBodyForm&, CollisionCallback&);

	static bool CheckCollision_Broad(components::PhysicsBody* body_a, components::PhysicsBody* body_b);

	static void SolveCircleVsCircle(
		PhysicsBodyForm& form_a,
		PhysicsBodyForm& form_b,
		CollisionCallback &callback);

	static void SolveRectangleVsRectangle(
		PhysicsBodyForm& form_a,
		PhysicsBodyForm& form_b,
		CollisionCallback& callback);
private:
	Solver();
};

_R2D_NAMESPACE_END_
