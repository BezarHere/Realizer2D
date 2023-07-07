#pragma once
#include "global.h"
#include "base.h"
#include "../Collidors.h"


_R2D_COMP_NS_START_

class PhysicsBody : public ObjectComponent
{
	friend class PhysicsServer;
public:
	PhysicsBody();

	void addCollidor(CollidorShape* collidor);

	std::vector<CollidorShape*>& getCollidors();
	const std::vector<CollidorShape*>& getCollidors() const;

	CollidorShape* getCollidor(size_t index);
	const CollidorShape* getCollidor(size_t index) const;

	void setCollidor(size_t index, CollidorShape* collidor);

	virtual PhysicsBodyType getType() const = 0;
	virtual bool isStatic() const = 0;

	void setActive(bool active) override;

	virtual void updateCache();

	const AABB& getCollectiveAABB() const;

protected:

	void ownerDetachedCallback() override;
	void ownerAtachedCallback() override;

private:
	void _registerMe();
	void _unregisterMe();

protected:
	bool m_registered{ false };
	// no good
	std::vector<CollidorShape*> m_collidors;
	AABB m_collectiveAABB{};
};

class StaticBody : public PhysicsBody
{
public:

	PhysicsBodyType getType() const override;
	bool isStatic() const override;

};

// no angular velocity only linear velocity
class KinematicBody : public PhysicsBody
{
public:


	PhysicsBodyType getType() const override;

	bool isStatic() const override;

private:
	real_t m_mass{ 1.0f };
	Vector2 m_linearVelocity{ 0.0f, 0.0f };
};

class DynamicBody : public KinematicBody
{
public:
	PhysicsBodyType getType() const override;

private:
};

_R2D_COMP_NS_END_