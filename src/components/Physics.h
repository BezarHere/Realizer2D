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

	virtual void applyForce(Vector2 force) = 0;

	// a body should sleep when it's not real needed for the moment to conserve performance
	// sleeping bodies wont be updated unless there is a collision
	virtual void setSleep(bool sleep);
	bool getSleep() const;

	uint32_t getSingleton() const override;

	virtual Vector2 calculatePentrationOffset(Vector2 pent, Vector2 cont) const;

protected:
	void onOwnerDetached() override;
	void onOwnerAtached() override;
	void entredScene() override;
	void exitedScene() override;

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
	
	void applyForce(Vector2 force) override;

	bool isStatic() const override;

	Vector2 calculatePentrationOffset(Vector2 pent, Vector2 cont) const override;
};

// no angular velocity only linear velocity
class KinematicBody : public PhysicsBody
{
public:
	PhysicsBodyType getType() const override;
	bool isStatic() const override;

	void applyForce(Vector2 force) override;

	Vector2 getLinearVelocity() const;
	void setLinearVelocity(Vector2 vel);

	real_t getMass() const;
	void setMass(real_t mass);

	// not including angular speed
	real_t getEnergy() const;

	Vector2 calculatePentrationOffset(Vector2 pent, Vector2 cont) const override;
private:
	real_t m_mass{ 1.0f };
	Vector2 m_linearVelocity{ 0.0f, 0.0f };
};

class DynamicBody : public KinematicBody
{
public:
	PhysicsBodyType getType() const override;

	void applyForce(Vector2 force, Vector2 contact_point);
	void applyTorque(real_t power);

	real_t getAngularVelocity() const;
	void setAngularVelocity(real_t mass);

	Vector2 calculatePentrationOffset(Vector2 pent, Vector2 cont) const override;
private:
	real_t m_angularVelocity{ 1.0f };
};

_R2D_COMP_NS_END_