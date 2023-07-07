#pragma once
#include "global.h"
#include "CollisionShape.h"
#include "components/Physics.h"

_R2D_NAMESPACE_START_

enum class PhysicsObjectType : uint8_t
{
	Static, // no velocity or rotation required
	Dynamic, // no rotation required
	Rigid
};

class PhysicsServer
{
public:

	void solve(components::PhysicsBody &body_a, components::PhysicsBody &body_b);

private:
	PhysicsServer();
private:
	typedef struct
	{
		std::vector<components::PhysicsBody*> bodies;
	} PhyCellCollection_t;

	std::vector<components::PhysicsBody*> m_objects;
	std::unordered_map<Point2, PhyCellCollection_t> m_phyObjectCells;
};

_R2D_NAMESPACE_END_