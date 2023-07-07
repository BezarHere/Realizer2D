#pragma once
#include "global.h"
#include "Object.h"
#include "Solver.h"

_R2D_NAMESPACE_START_

struct _STRUCT__PhysicsServer___Init;
class PhysicsServer final
{
	typedef components::PhysicsBody PhysicsBody;
	friend _STRUCT__PhysicsServer___Init;
public:


	static void Solve(PhysicsBody *body_a, PhysicsBody *body_b);

	// unsafe: only call if you know what are you doing
	static bool _has_registeryPhyComp(PhysicsBody* comp);
	// unsafe: only call if you know what are you doing
	static void _registerPhyComp(PhysicsBody* comp);
	// unsafe: only call if you know what are you doing
	static void _unregisterPhyComp(PhysicsBody* comp);

	static void ResolveCollision(PhysicsBody* body_a, PhysicsBody* body_b);

private:
	static void Init();

private:
	typedef struct
	{
		std::vector<components::PhysicsBody*> bodies;
	} PhyCellCollection_t;

	static inline std::vector<components::PhysicsBody*> s_objects;
	static inline std::unordered_map<Point2, PhyCellCollection_t> s_phyObjectCells;
};

#undef SOLVE_FUNC_DEF
_R2D_NAMESPACE_END_