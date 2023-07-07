#pragma once
#include "base.h"
#include "../CollisionShape.h"

_R2D_COMP_NS_START_

class PhysicsBody : public ObjectComponent
{
public:

	CollidorShape* getCollidor();
	const CollidorShape* getCollidor() const;
	void setCollidor(CollidorShape* collidor);

protected:
	CollidorShape* m_collidor;
};


typedef PhysicsBody StaticBody;

// sometimes called kinematic body
class DynamicBody : public PhysicsBody
{

};

class RigidBody : public DynamicBody
{

};

_R2D_COMP_NS_END_