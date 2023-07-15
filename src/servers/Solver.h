#pragma once
#include "global.h"
#include "scene/Collidors.h"
#include "components/Physics.h"

_R2D_NAMESPACE_START_

struct CollisionCallback
{
	bool collided;
	Vector2 seprating_axis;
	float seprating_distance;
	Vector2 collision_point;
};

struct PhysicsBodyForm
{
	PhysicsBodyForm(components::PhysicsBody* p_body, const CollidorShape* p_col, const Transform2D& p_trans);
	components::PhysicsBody* body;
	const CollidorShape* collidor;
	const Transform2D& transform;
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
