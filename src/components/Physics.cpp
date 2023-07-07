#include "global.h"
#include "../PhysicsServer.h"
#include "../Object.h"
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

void PhysicsBody::ownerDetachedCallback()
{
  if (isActive())
   _unregisterMe();
}

void PhysicsBody::ownerAtachedCallback()
{
  if (isActive())
    _registerMe();
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


PhysicsBodyType KinematicBody::getType() const
{
  return PhysicsBodyType::Dynamic;
}


bool StaticBody::isStatic() const
{
  return true;
}

bool KinematicBody::isStatic() const
{
  return false;
}

PhysicsBodyType DynamicBody::getType() const
{
  return PhysicsBodyType::Rigid;
}

PhysicsBodyType StaticBody::getType() const
{
  return PhysicsBodyType::Static;
}

_R2D_COMP_NS_END_


