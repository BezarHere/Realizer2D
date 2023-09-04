#include "global.h"
#include "solver.h"
#include "utils/algorithms.h"
#include "scene/Object.h"

#define out
#define in
#define forceinline __forceinline

using namespace r2d::components;

_R2D_NAMESPACE_START_

void CollisionCallback::invalidate()
{
	collision = false;
}

CollisionCallback::operator bool() const
{
	return collision;
}

bool CollisionCallback::operator!() const
{
	return !collision;
}

struct Seg
{
	real_t a, b;

	forceinline operator Vector2() const
	{
		return { a, b };
	}

	forceinline real_t size() const
	{
		return b - a;
	}

	forceinline real_t center() const
	{
		return (b + a) / 2.0f;
	}

};

struct Shadow
{
	real_t startValue{INFINITY}, endValue{-INFINITY};
	int32_t startIndex{-1}, endIndex{-1};

	forceinline bool intersects(const Shadow &other) const
	{
		return other.endValue >= startValue && endValue >= other.startValue;
	}

	forceinline Seg intersection(const Shadow &other) const
	{
		return { MAX(other.startValue, startValue), MIN(other.endValue, endValue) };
	}

	forceinline real_t size() const
	{
		return endValue - startValue;
	}

	forceinline real_t center() const
	{
		return (endValue + startValue) / 2.0f;
	}
};

struct ShadowPlane
{
	Shadow shadows[ 2 ];
	Vector2 normal;
};

// returns true if there is an intersection
forceinline bool M_SAT_CheckCast(const Points_t &points_a, const Points_t &points_b, Vector2 normal, out ShadowPlane &plane)
{
#ifdef _DEBUG
	assert(plane.normal.lengthSquared() == 1.0f); // length == 1.0 <=> lengthSquared == 1.0
#endif // _DEBUG
	real_t dot_v;
	Shadow &shadow_a = plane.shadows[ 0 ];
	Shadow &shadow_b = plane.shadows[ 1 ];
	
	for (int32_t i{}; i < points_a.size(); i++)
	{
		dot_v = points_a[ i ].dot(normal);
		if (shadow_a.startValue > dot_v)
		{
			shadow_a.startValue = dot_v;
			shadow_a.startIndex = i;
		}
		else if (shadow_a.endValue < dot_v)
		{
			shadow_a.endValue = dot_v;
			shadow_a.endIndex = i;
		}
	}

	for (int32_t i{}; i < points_b.size(); i++)
	{
		dot_v = points_b[ i ].dot(normal);
		if (shadow_b.startValue > dot_v)
		{
			shadow_b.startValue = dot_v;
			shadow_b.startIndex = i;
		}
		else if (shadow_b.endValue < dot_v)
		{
			shadow_b.endValue = dot_v;
			shadow_b.endIndex = i;
		}
	}

	return shadow_a.intersects(shadow_b);
}

// returns middle point
forceinline Vector2 M_PreprocessPoints(const Transform2D &trans, in out Points_t &points)
{
	Vector2 mid{ 0.0f, 0.0f };
	for (Vector2 &p : points)
	{
		p = trans.transformPoint(p);
		mid += p;
	}
	mid /= (float)points.size();
	return mid;
}

// returns middle point
forceinline Points_t M_PreprocessPoints(const Transform2D &trans, in Points_t points, out Vector2 &center)
{
	center = Vector2(0.0f, 0.0f);
	for (Vector2 &p : points)
	{
		p = trans.transformPoint(p);
		center += p;
	}
	center /= (float)points.size();
	return points;
}

forceinline void M_ProcessSAT_BindPlanesToCallback(const Points_t &points_a, const Points_t &points_b,
																									 const ShadowPlane &plane_a, const ShadowPlane &plane_b,
																									 Vector2 center, out CollisionCallback &callback)
{
	// contact point
	const Seg plane_a_intersection{ plane_a.shadows[ 0 ].intersection(plane_a.shadows[ 1 ]) };
	const Seg plane_b_intersection{ plane_b.shadows[ 0 ].intersection(plane_b.shadows[ 1 ]) };

	const real_t center_shadow_normal_a = plane_a.normal.dot(center);
	const real_t center_shadow_normal_b = plane_b.normal.dot(center);

	const Vector2 c_offset_to_cont_point{
		(-plane_a.normal * (center_shadow_normal_a - plane_a_intersection.center())) +
		(-plane_b.normal * (center_shadow_normal_b - plane_b_intersection.center()))
	};

	const Vector2 cont_point{ center + c_offset_to_cont_point };
	callback.contact = cont_point;

	// pentration

	// tangents always pointing to the genral direction of the contact point
	const Vector2 plane_tangent_a =
		plane_a.normal.tangent().dot(cont_point) > 0.0f ? plane_a.normal.tangent() : -plane_a.normal.tangent();
	const Vector2 plane_tangent_b = 
		plane_b.normal.tangent().dot(cont_point) > 0.0f ? plane_b.normal.tangent() : -plane_b.normal.tangent();

	const real_t contact_distance_a = std::abs(plane_tangent_a.dot(cont_point));
	const real_t contact_distance_b = std::abs(plane_tangent_b.dot(cont_point));

	const real_t deepest_point_distance_a = std::abs(plane_tangent_a.dot(points_a[ plane_b.shadows[ 0 ].startIndex ]));
	const real_t deepest_point_distance_b = std::abs(plane_tangent_b.dot(points_b[ plane_a.shadows[ 1 ].startIndex ]));

	const real_t pentartion_distance_in_a = deepest_point_distance_a - contact_distance_a;
	const real_t pentartion_distance_in_b = deepest_point_distance_b - contact_distance_b;

	// a is inside b
	if (pentartion_distance_in_a < pentartion_distance_in_b)
	{
		callback.pentration = pentartion_distance_in_a * -plane_tangent_a;
	}
	else
	{
		callback.pentration = pentartion_distance_in_b * plane_tangent_b;
	}
}

forceinline void M_ProcessSAT(const Points_t &points_a, const Points_t &normals_a, const Vector2 center_a,
															const Points_t &points_b, const Points_t &normals_b, const Vector2 center_b,
															out CollisionCallback &callback)
{
	constexpr real_t NormalSearchTolarence = Epsilon * 10.0f;
	constexpr real_t NormalSearchMaximum = 1.0f - NormalSearchTolarence;
	// base case: the two polys are in the same place
	if (center_a == center_b)
	{
		callback.collision = false;
		return;
	}
	
	// center between the two polygons
	Vector2 center{ (center_b + center_a) / 2.0f };

	// pointing to A from the center
	Vector2 axis_b{ center_a - center },  axis_a{ -axis_b };
	
	// best axis dot product value
	real_t axis_normal_a_best{ -1.0f }, axis_normal_b_best{ -1.0f };
	// best normal facing other polygon
	Vector2 axis_normal_a, axis_normal_b;

	// temp for reducing stack-alloc
	real_t axis_normal_value;
	

	ShadowPlane planes[ 2 ];

	// compute the smallest shape first
	// if there is a gap, no need to compute the other(complex) one
	if (points_a.size() <= points_b.size())
	{
		for (const Vector2 &n : normals_a)
		{
			axis_normal_value = n.dot(axis_a);
			if (axis_normal_value > axis_normal_a_best)
			{
				axis_normal_a_best = axis_normal_value;
				axis_normal_a = n;

				// best axis tolarated, no more searching
				if (axis_normal_value >= NormalSearchMaximum)
					break;
			}
		}
		planes[ 0 ].normal = axis_normal_a;

		if (!M_SAT_CheckCast(points_a, points_b, axis_normal_a, planes[ 0 ]))
		{
			callback.invalidate();
			return;
		}

		for (const Vector2 &n : normals_b)
		{
			axis_normal_value = n.dot(axis_b);
			if (axis_normal_value > axis_normal_b_best)
			{
				axis_normal_b_best = axis_normal_value;
				axis_normal_b = n;

				// best axis tolarated, no more searching
				if (axis_normal_value >= NormalSearchMaximum)
					break;
			}
		}
		planes[ 1 ].normal = axis_normal_b;

		if (!M_SAT_CheckCast(points_a, points_b, axis_normal_b, planes[ 1 ]))
		{
			callback.invalidate();
			return;
		}
	}
	else
	{
		for (const Vector2 &n : normals_b)
		{
			axis_normal_value = n.dot(axis_b);
			if (axis_normal_value > axis_normal_b_best)
			{
				axis_normal_b_best = axis_normal_value;
				axis_normal_b = n;

				// best axis tolarated, no more searching
				if (axis_normal_value >= NormalSearchMaximum)
					break;
			}
		}
		planes[ 1 ].normal = axis_normal_b;

		if (!M_SAT_CheckCast(points_a, points_b, axis_normal_b, planes[ 1 ]))
		{
			callback.invalidate();
			return;
		}

		for (const Vector2 &n : normals_a)
		{
			axis_normal_value = n.dot(axis_a);
			if (axis_normal_value > axis_normal_a_best)
			{
				axis_normal_a_best = axis_normal_value;
				axis_normal_a = n;

				// best axis tolarated, no more searching
				if (axis_normal_value >= NormalSearchMaximum)
					break;
			}
		}
		planes[ 0 ].normal = axis_normal_a;

		if (!M_SAT_CheckCast(points_a, points_b, axis_normal_a, planes[ 0 ]))
		{
			callback.invalidate();
			return;
		}
	}
	callback.collision = true;
	M_ProcessSAT_BindPlanesToCallback(points_a, points_b, planes[ 0 ], planes[ 1 ], center, callback);
}


PhysicsBodyForm::PhysicsBodyForm(components::PhysicsBody* body, const CollidorShape* shape, const Transform2D& transform)
	: m_body(body), m_shape(shape), m_transform(transform), m_transformedPoints(shape->getPoints())
	
{
}

void PhysicsBodyForm::tryGenerate()
{
	if (m_generated)
		return;
	generate();
	m_generated = true;
}

void PhysicsBodyForm::generate()
{
	m_center = M_PreprocessPoints(m_transform, m_transformedPoints);
	m_transformedNormals = m_shape->getNormals();
	real_t shape_angle_rad = deg2rad(m_body->getOwner()->getGlobalRotation());
	real_t tmp_angle;
	for (Vector2 &p : m_transformedNormals)
	{
#ifdef _DEBUG
		assert(p.lengthSquared() == 1.0f);
#endif // _DEBUG

		tmp_angle = p.angle() + shape_angle_rad;
		p.x = std::cos(tmp_angle);
		p.y = std::sin(tmp_angle);
	}
}

const components::PhysicsBody *PhysicsBodyForm::getBody() const
{
	return m_body;
}

const CollidorShape *PhysicsBodyForm::getShape() const
{
	return m_shape;
}

const Transform2D &PhysicsBodyForm::getTransform() const
{
	return m_transform;
}

const Points_t &PhysicsBodyForm::getTransformedPoints() const
{
	return m_transformedPoints;
}

const Points_t &PhysicsBodyForm::getTransformedNormals() const
{
	return m_transformedNormals;
}

Vector2 PhysicsBodyForm::getCenter() const
{
	return m_center;
}

bool Solver::CheckCollision_Broad(components::PhysicsBody* body_a, components::PhysicsBody* body_b)
{
	return body_a->getCollectiveAABB().intersects(body_b->getCollectiveAABB());
}

void Solver::SolveCircleVsCircle(PhysicsBodyForm& form_a, PhysicsBodyForm& form_b, CollisionCallback& callback)
{
	const CircleCollidor *shape_a = (const CircleCollidor *)form_a.getShape();
	const CircleCollidor *shape_b = (const CircleCollidor *)form_b.getShape();
	//? FIXME: getOffset isn't related to global transform, might do unpredictable things
	const Vector2 gpos_a = form_a.getBody()->getOwner()->getGlobalPosition() + shape_a->getOffset();
	const Vector2 gpos_b = form_b.getBody()->getOwner()->getGlobalPosition() + shape_b->getOffset();
	const real_t gdis = gpos_a.distance(gpos_b);
	if (gdis < shape_a->getRadius() + shape_b->getRadius())
	{
		callback.collision = true;
		callback.pentration = -gpos_a.direction(gpos_b) * ((shape_a->getRadius() + shape_b->getRadius()) - gdis);
		return;
	}
	callback.invalidate();
}

void Solver::SolveRectangleVsRectangle(PhysicsBodyForm& form_a, PhysicsBodyForm& form_b, CollisionCallback& callback)
{
	M_ProcessSAT(form_a.getTransformedPoints(), form_a.getTransformedNormals(), form_a.getCenter(),
							 form_b.getTransformedPoints(), form_b.getTransformedNormals(), form_b.getCenter(),
							 callback);
}

_R2D_NAMESPACE_END_