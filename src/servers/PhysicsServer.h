#pragma once
#include <stack>
#include "global.h"
#include "scene/Object.h"
#include "scene/Collidors.h"
#include "Solver.h"
#include "R2DInit.h"

_R2D_NAMESPACE_START_
#define global static inline

class PhysicsServer final
{
	typedef components::PhysicsBody PhysicsBody;
	friend class R2DInit;
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
	typedef std::pair<components::PhysicsBody *, components::PhysicsBody *> PhyBodyPair_t;
	typedef std::pair<PhysicsBody *, const CollidorShape *> CollidorKey_t;
	typedef struct
	{
		std::vector<components::PhysicsBody*> bodies;
		mutable std::unordered_set<PhyBodyPair_t> checks;
	} CollisionGridCell_t;

	static PhysicsBodyForm &getForm(CollidorKey_t key, const Transform2D &transform);

	static void populateGrid();

	static ErrorCode Init();

private:
	global std::unordered_map<CollidorKey_t, PhysicsBodyForm> s_formsCache{};
	global constexpr int CollisionGridCellSize{ 64 };
	global std::vector<components::PhysicsBody*> s_bodies;
	global std::unordered_map<IVector2, CollisionGridCell_t> s_collisionGrid;
	global std::unordered_set<IVector2> s_availableCollisionGridCells; // for perfoment searching, i guess
	global real_t s_deltaTime{ 1.0f };
	global real_t s_stepDeltaTime{ 1.0f };
	global int s_updateSteps{ 1 }; // higher = more accurate physics = more cost on cpu
};


#undef global
_R2D_NAMESPACE_END_
