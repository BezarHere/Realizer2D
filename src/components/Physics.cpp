#include "global.h"
#include "servers/PhysicsServer.h"
#include "scene/Object.h"
#include "Physics.h"



_R2D_COMP_NS_START_

PhysicsBody::PhysicsBody()
{
  updateCache();
}

void PhysicsBody::addCollidor(CollidorShape* collidor)
{
  m_collidors.push_back(collidor);
}

std::vector<CollidorShape*>& PhysicsBody::getCollidors()
{
  return m_collidors;
}

const std::vector<CollidorShape*>& PhysicsBody::getCollidors() const
{
  return m_collidors;
}

CollidorShape* PhysicsBody::getCollidor(size_t index)
{
  return m_collidors[index];
}

const CollidorShape* PhysicsBody::getCollidor(size_t index) const
{
  return m_collidors[index];
}

void PhysicsBody::setCollidor(size_t index, CollidorShape* collidor)
{
  m_collidors[index] = collidor;
}


void PhysicsBody::setActive(bool active)
{
  if (active == isActive())
    return;
  ObjectComponent::setActive(active);
  if (getOwner())
  {
    if (active)
    {
      _registerMe();
    }
    else
    {
      _unregisterMe();
    }
  }
}

void PhysicsBody::updateCache()
{
  m_collectiveAABB = { INFINITY, INFINITY, -INFINITY, -INFINITY };
  PEEK("updated cache");
  PEEK(m_collidors.size());
  for (const CollidorShape* shape : m_collidors)
  {
    m_collectiveAABB.merge(shape->getAABB());
   PEEK(shape->getAABB());
  }
}

const AABB& PhysicsBody::getCollectiveAABB() const
{
  return m_collectiveAABB;
}

void PhysicsBody::setSleep(bool sleep)
{
  m_sleep = sleep;
}

bool PhysicsBody::getSleep() const
{
    return m_sleep;
}

uint32_t PhysicsBody::getSingleton() const
{
  return 1U;
}

Vector2 PhysicsBody::calculatePentrationOffset(Vector2 pent, Vector2 cont) const
{
  return Vector2{};
}

void PhysicsBody::onOwnerDetached()
{
  if (isActive())
   _unregisterMe();
}

void PhysicsBody::onOwnerAtached()
{
  if (isActive())
    _registerMe();
}

void PhysicsBody::entredScene()
{
}

void PhysicsBody::exitedScene()
{
}

void PhysicsBody::_registerMe()
{
  m_registered = true;
  _R2D_ PhysicsServer::_registerPhyComp(this);
}

void PhysicsBody::_unregisterMe()
{
  m_registered = false;
  _R2D_ PhysicsServer::_unregisterPhyComp(this);
}

PhysicsBodyType StaticBody::getType() const
{
  return PhysicsBodyType::Static;
}

void StaticBody::applyForce(Vector2 force)
{
  return;
}

bool StaticBody::isStatic() const
{
  return true;
}

Vector2 StaticBody::calculatePentrationOffset(Vector2 pent, Vector2 cont) const
{
  return Vector2{};
}

PhysicsBodyType KinematicBody::getType() const
{
  return PhysicsBodyType::Dynamic;
}

bool KinematicBody::isStatic() const
{
  return false;
}

void KinematicBody::applyForce(Vector2 force)
{
  m_linearVelocity += force / m_mass;
}

Vector2 KinematicBody::getLinearVelocity() const
{
  return m_linearVelocity;
}

void KinematicBody::setLinearVelocity(Vector2 vel)
{
  m_linearVelocity = vel;
}

real_t KinematicBody::getMass() const
{
    return m_mass;
}

void KinematicBody::setMass(real_t mass)
{
  m_mass = mass;
}

real_t KinematicBody::getEnergy() const
{
  return getLinearVelocity().lengthSquared() * getMass() / 2.0f;
}

Vector2 KinematicBody::calculatePentrationOffset(Vector2 pent, Vector2 cont) const
{
  return pent;
}

PhysicsBodyType DynamicBody::getType() const
{
  return PhysicsBodyType::Rigid;
}

void DynamicBody::applyForce(Vector2 force, Vector2 contact_point)
{
  const Vector2 gpos = getOwner()->getGlobalPosition();
  const Vector2 cont_normal = (gpos - contact_point).normalized();
  const real_t conpos_force_aligment = force.dot(cont_normal);
  KinematicBody::applyForce(force * -conpos_force_aligment);
  //? FIXME: Angular vel
}

void DynamicBody::applyTorque(real_t power)
{
  m_angularVelocity += power / getMass();
}

real_t DynamicBody::getAngularVelocity() const
{
    return m_angularVelocity;
}

void DynamicBody::setAngularVelocity(real_t mass)
{
  m_angularVelocity = mass;
}

Vector2 DynamicBody::calculatePentrationOffset(Vector2 pent, Vector2 cont) const
{
  // FIXME
  return pent;
}

_R2D_COMP_NS_END_

_R2D_register_comp(PhysicsBody);
_R2D_register_comp(StaticBody);
_R2D_register_comp(KinematicBody);
_R2D_register_comp(DynamicBody);
