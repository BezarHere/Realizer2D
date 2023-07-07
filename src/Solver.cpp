#include "global.h"
#include "solver.h"

using namespace r2d::components;

_R2D_NAMESPACE_START_

PhysicsBodyForm::PhysicsBodyForm(components::PhysicsBody* p_body, const CollidorShape* p_col, const Transform2D& p_trans)
	: body(p_body), collidor(p_col), transform(p_trans)
{

}

bool Solver::CheckCollision_Broad(components::PhysicsBody* body_a, components::PhysicsBody* body_b)
{
	return body_a->getCollectiveAABB().intersects(body_b->getCollectiveAABB());
}

void Solver::SolveCircleVsCircle(PhysicsBodyForm& form_a, PhysicsBodyForm& form_b, CollisionCallback& callback)
{
}

void Solver::SolveRectangleVsRectangle(PhysicsBodyForm& form_a, PhysicsBodyForm& form_b, CollisionCallback& callback)
{
}

_R2D_NAMESPACE_END_