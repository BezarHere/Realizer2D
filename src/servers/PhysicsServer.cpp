#include "global.h"
#include "PhysicsServer.h"

_R2D_NAMESPACE_START_

struct _CollidorShapeTypeKey
{
	_CollidorShapeTypeKey(CollidorShapeType pa, CollidorShapeType pb)
		: a(pa), b(pb)
	{}

	bool operator==(const _CollidorShapeTypeKey other) const
	{
		return a == other.a && b == other.b;
	}

	operator std::pair<CollidorShapeType, CollidorShapeType>()
	{
		return { a, b };
	}

	CollidorShapeType a;
	CollidorShapeType b;
};

_R2D_NAMESPACE_END_

template <>
struct std::hash<_R2D_ _CollidorShapeTypeKey> {
	inline size_t operator()(_R2D_ _CollidorShapeTypeKey obj) const {
		return (size_t)obj.a + (((size_t)obj.b) << 8);
	}
};


_R2D_NAMESPACE_START_

using components::KinematicBody;
using components::DynamicBody;

#define SolverFunctionEntry(type_a, type_b, function_ptr) \
{{(CollidorShapeType)(type_a > type_b ? type_b : type_a), (CollidorShapeType)(type_a < type_b ? type_b : type_a)}, (Solver::SolverFunction)(function_ptr)}

static inline std::unordered_map<_CollidorShapeTypeKey, Solver::SolverFunction> solverMap
{
	SolverFunctionEntry(
		CollidorShapeType::Circle,
		CollidorShapeType::Circle,
		Solver::SolveCircleVsCircle),
	SolverFunctionEntry(
		CollidorShapeType::Rectangle,
		CollidorShapeType::Rectangle,
		Solver::SolveRectangleVsRectangle)
};

static constexpr size_t _PhyObjectsAreanaDesiredSize = 0xfffff; // 2^20 - 1 (~1 mb) or 131071 64x ptrs or 262143 86x ptrs

// A's force factor is mul by this but B's force factor should be mul by 1.0f - this
__forceinline real_t f_pushRatio(const KinematicBody *a, const KinematicBody *b)
{
	return b->getMass() / (a->getMass() + b->getMass());
}


ErrorCode PhysicsServer::Init()
{
	s_bodies.reserve(_PhyObjectsAreanaDesiredSize / sizeof(void *));
	s_formsCache.reserve(_PhyObjectsAreanaDesiredSize / sizeof(void *));
	return ErrorCode::Ok;
}


void PhysicsServer::update(real_t delta)
{
	s_deltaTime = delta;
	s_stepDeltaTime = delta / s_updateSteps;
	for (int i{ 0 }; i < s_updateSteps; i++)
		updateStep();
}

void PhysicsServer::updateStep()
{
	populateGrid();
	for (const auto& kv : s_collisionGrid)
	{
		for (PhysicsBody* body_a : kv.second.bodies)
		{
			for (PhysicsBody* body_b : kv.second.bodies)
			{
				// TODO: check if having an unordered_set to check back-to-back collision is better then just checking
				if (body_a == body_b || kv.second.checks.contains({ body_a, body_b }))
					continue;
				Solve(body_a, body_b);
				kv.second.checks.insert({ body_b, body_a });
			}
		}
	}
	s_formsCache.clear();
}

void PhysicsServer::Solve(components::PhysicsBody* body_a, components::PhysicsBody* body_b)
{
	// two static bodies can't collide
	if (body_a->isStatic() == true && body_a->isStatic())
		return;


	// low cost detection
	if (!Solver::CheckCollision_Broad(body_a, body_b))
		return;

	// yeah... my implementation of SAT can't handle an A type bigger then the B type
	// like dynamic vs static is not possible but static vs dynamic is, wich has no diffrence (as i know)
	if (body_a->getType() > body_b->getType())
	{
		swapval(body_a, body_b);
	}
	
	ResolveCollision(body_a, body_b);
}

bool PhysicsServer::_has_registeryPhyComp(components::PhysicsBody* comp)
{
	return std::find(s_bodies.begin(), s_bodies.end(), comp) != s_bodies.end();
}

void PhysicsServer::_registerPhyComp(components::PhysicsBody* comp)
{
	s_bodies.push_back(comp);
}

void PhysicsServer::_unregisterPhyComp(components::PhysicsBody* comp)
{
	s_bodies.erase(std::find(s_bodies.begin(), s_bodies.end(), comp));
}

// TODO: add responses after fixing SAT
// bodies A & B types should not be reversed (e.g. body_a is a dynamic & body_b is a static)
void PhysicsServer::ResolveCollision(PhysicsBody* body_a, PhysicsBody* body_b)
{
	Transform2D trans_a = body_a->getOwner()->getGlobalTransform();
	Transform2D trans_b = body_b->getOwner()->getGlobalTransform();
	

	for (const auto* col_a : body_a->getCollidors())
	{
		for (const auto* col_b : body_b->getCollidors())
		{
			PhysicsBodyForm &form_a = getForm({ body_a, col_a }, trans_a);
			PhysicsBodyForm &form_b = getForm({ body_b, col_b }, trans_b);

			CollisionCallback callback{};

			solverMap.at({ col_a->getType(), col_b->getType() })(form_a, form_b, callback);

			if (callback)
			{
				// all objects are dynamic, therefor the two objects should be pushed
				// given that the body A is always a lower type then body B, we can be asure that
				// if body A isn't static, body B should so be non static
				const bool dynamic_collision = !body_a->isStatic();
				const real_t pentration_offset_ratio = dynamic_collision ? f_pushRatio((const KinematicBody *)body_a, (const KinematicBody *)body_b) : 0.0f;

				form_a.applyedForce += body_a->calculatePentrationOffset(
					callback.pentration * pentration_offset_ratio, callback.contact
				);

				form_b.applyedForce += body_b->calculatePentrationOffset(
					-callback.pentration * (1.0f - pentration_offset_ratio), callback.contact
				);

			}
		}
	}
}

//! FIXME: this ain't optimal
PhysicsBodyForm &PhysicsServer::getForm(CollidorKey_t key, const Transform2D &transform)
{
	if (!s_formsCache.contains(key))
	{
		s_formsCache.insert({ key, PhysicsBodyForm(key.first, key.second, transform)});
	}
	return s_formsCache.at(key);
}

void PhysicsServer::populateGrid()
{
	s_collisionGrid.clear();
	s_availableCollisionGridCells.clear();

	for (PhysicsBody* body : s_bodies)
	{
		body->updateCache();
		const AABB aabb = body->getCollectiveAABB() / (float)CollisionGridCellSize;
		const IVector2 grid_position{ (int)aabb.left, (int)aabb.top };
		for (int x{ 0 }; x < ceil(aabb.width); x++)
		{
			for (int y{ 0 }; y < ceil(aabb.height); y++)
			{
				IVector2 grid_pos{ x + grid_position.x, y + grid_position.y };
				s_collisionGrid[grid_pos].bodies.push_back(body);
				s_availableCollisionGridCells.insert(grid_pos);
			}
		}
	}
}



_R2D_NAMESPACE_END_