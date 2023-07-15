#pragma once
#include <stack>
#include "global.h"
#include "scene/Object.h"
#include "scene/Collidors.h"
#include "Solver.h"

_R2D_NAMESPACE_START_
#define global static inline

struct _STRUCT__PhysicsServer___Init;
class PhysicsServer final
{
	typedef components::PhysicsBody PhysicsBody;
	friend _STRUCT__PhysicsServer___Init;
public:
	
	/////////////////////////////
	
	/////////////////////////////

	void update(real_t delta);
	void updateStep();

	static void Solve(PhysicsBody *body_a, PhysicsBody *body_b);

	// unsafe: only call if you know what are you doing
	static bool _has_registeryPhyComp(PhysicsBody* comp);
	// unsafe: only call if you know what are you doing
	static void _registerPhyComp(PhysicsBody* comp);
	// unsafe: only call if you know what are you doing
	static void _unregisterPhyComp(PhysicsBody* comp);

	static void ResolveCollision(PhysicsBody* body_a, PhysicsBody* body_b);

private:
	static void populateGrid();

	static void Init();

private:
	typedef struct
	{
		std::vector<components::PhysicsBody*> bodies;
	} CollisionGridCell_t;
	global constexpr int CollisionGridCellSize{ 64 };
	global std::vector<components::PhysicsBody*> s_bodies;
	global std::unordered_map<Point2, CollisionGridCell_t> s_collisionGrid;
	global std::unordered_set<Point2> s_availableCollisionGridCells; // for perfoment searching, i guess
	global real_t s_deltaTime{ 1.0f };
	global real_t s_stepDeltaTime{ 1.0f };
	global int s_updateSteps{ 1 }; // higher = more accurate physics = more cost on cpu
};


#undef global
_R2D_NAMESPACE_END_