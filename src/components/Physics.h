#pragma once
#include "global.h"
#include "base.h"
#include "scene/Collidors.h"


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

	// a body should sleep when it's not real needed for the moment to conserve performance
	// sleeping bodies wont be updated unless there is a collision
	virtual void setSleep(bool sleep);
	bool getSleep() const;


protected:

	void ownerDetachedCallback() override;
	void ownerAtachedCallback() override;

private:
	void _registerMe();
	void _unregisterMe();

protected:
	bool m_registered{ false };
	bool m_sleep{ true };
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