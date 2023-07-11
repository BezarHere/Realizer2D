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

#define SolverFunctionEntry(type_a, type_b, function_ptr) \
{{(CollidorShapeType)(type_a > type_b ? type_b : type_a), (CollidorShapeType)(type_a < type_b ? type_b : type_a)}, (Solver::SolverFunction)(function_ptr)}


static inline std::unordered_map<_CollidorShapeTypeKey, Solver::SolverFunction> solverMap
{
	SolverFunctionEntry(
		CollidorShapeType::Circle,
		CollidorShapeType::Circle,
		Solver::SolveCircleVsCircle)
};

static constexpr size_t _PhyObjectsAreanaDesiredSize = 0xfffff; // 2^20 - 1 (~1 mb) or 131071 64x ptrs or 262143 86x ptrs



struct _STRUCT__PhysicsServer___Init
{
	_STRUCT__PhysicsServer___Init()
	{
		PhysicsServer::Init();
#ifdef R2D_VERBOSE
		std::cout << "_STRUCT__PhysicsServer___Init\n";
#endif // R2D_VERBOSE
	}
} __PhysicsServer___Init;

void PhysicsServer::Init()
{
	s_objects.reserve(_PhyObjectsAreanaDesiredSize / sizeof(void*));
}


void PhysicsServer::update(real_t delta)
{
}

void PhysicsServer::updateStep(real_t delta)
{
}

void PhysicsServer::Solve(components::PhysicsBody* body_a, components::PhysicsBody* body_b)
{
	// two static bodies can't collide
	if (body_a->isStatic() == true && body_a->isStatic())
	{
		return;
	}

	// yeah...
	if (body_a->getType() > body_b->getType())
	{
		doswap(body_a, body_b);
	}

	// low cost detection
	if (!Solver::CheckCollision_Broad(body_a, body_b))
		return;

	ResolveCollision(body_a, body_b);
}

bool PhysicsServer::_has_registeryPhyComp(components::PhysicsBody* comp)
{
	return std::find(s_objects.begin(), s_objects.end(), comp) != s_objects.end();
}

void PhysicsServer::_registerPhyComp(components::PhysicsBody* comp)
{
	s_objects.push_back(comp);
}

void PhysicsServer::_unregisterPhyComp(components::PhysicsBody* comp)
{
	s_objects.erase(std::find(s_objects.begin(), s_objects.end(), comp));
}

void PhysicsServer::ResolveCollision(PhysicsBody* body_a, PhysicsBody* body_b)
{
	Transform2D trans_a = body_a->getOwner()->getGlobalTransform();
	Transform2D trans_b = body_b->getOwner()->getGlobalTransform();
	

	for (const auto* col_a : body_a->getCollidors())
	{
		for (const auto* col_b : body_b->getCollidors())
		{
			PhysicsBodyForm form_a{ body_a, col_a, trans_a };
			PhysicsBodyForm form_b{ body_b, col_b, trans_b };

			CollisionCallback callback{};

			solverMap.at({ col_a->getType(), col_b->getType() })(form_a, form_b, callback);
		}
	}
}



_R2D_NAMESPACE_END_