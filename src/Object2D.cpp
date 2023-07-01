#include "global.h"
#include "Object2D.h"
#include "Engine.h"

_R2D_NAMESPACE_START_

Object2D::Object2D()
	: engine{ Engine::Singleton() }
{
	assert_msg(engine, "can't create an Object2D without the engine starting first.");
	if (engine)engine->RegisterObject(this);
}

Object2D::~Object2D()
{
	engine->PopObject(this);
}

void Object2D::update()
{
	for (const std::shared_ptr<ObjectComponent2D>& comp : m_components)
	{
		comp->update();
	}
}

void Object2D::draw(sf::RenderTarget& target, sf::RenderStates state) const
{
	state.transform.combine(getTransform());
	for (const std::shared_ptr<ObjectComponent2D>& comp : m_components)
	{
		comp->draw(target, state);
	}
}

void Object2D::addChild(const std::shared_ptr<Object2D>& child)
{
	m_children.insert(child);
}

void Object2D::addChild(Object2D* child)
{
	addChild(std::shared_ptr<Object2D>(child));
}

bool Object2D::hasChild(const std::shared_ptr<Object2D>& child) const
{
	return m_children.find(child) == m_children.end();
}

bool Object2D::hasChild(const ObjID_t objid) const
{
	const auto objid_find_predicate = 
	[objid](std::shared_ptr<Object2D> p) {
		return p->getObjectID() == objid;
	};
	return std::find_if(m_children.begin(), m_children.end(), objid_find_predicate) == m_children.end();
}

bool Object2D::removeChild(const std::shared_ptr<Object2D>& child)
{
	return (bool)m_children.erase(child);
}

bool Object2D::removeChild(const ObjID_t objid)
{
	return removeChild(std::shared_ptr<Object2D>(getChild(objid)));
}

Object2D* Object2D::getChild(const ObjID_t objid) const
{
	const auto objid_find_predicate =
		[objid](std::shared_ptr<Object2D> p) {
		return p->getObjectID() == objid;
	};
	return (*std::find_if(m_children.begin(), m_children.end(), objid_find_predicate)).get();
}

void Object2D::installComponent(ObjectComponent2D* component)
{
	// can't install a component owned by another object (or reistall an already installed component)
	assert(component->m_obj == nullptr);
	component->m_obj = this;
	m_components.insert(std::shared_ptr<ObjectComponent2D>(component));
}

void Object2D::removeComponent(ObjectComponent2D* component)
{
	// can't remove a component not owned by this
	assert(component->m_obj == this);
	component->m_obj = nullptr;
	m_components.erase(std::shared_ptr<ObjectComponent2D>(component));
}

bool Object2D::hasComponent(ObjectComponent2D* component) const
{
	for (const auto& p : m_components)
	{
		if (p.get() == component)
			return true;
	}
	return false;
}

const std::unordered_set<std::shared_ptr<ObjectComponent2D>>& Object2D::getComponents() const
{
	return m_components;
}

void Object2D::onPreDelete()
{
	// what can i put here??
}

void Object2D::onRemovedFromScene()
{
	assert_msg(m_insideScene, "Can't remove an already removed object from the scene tree.");
	m_insideScene = false;
	for (const std::shared_ptr<Object2D>& p : m_children)
	{
		p->onRemovedFromScene();
	}
}

void Object2D::onAddedToScene()
{
	assert_msg(!m_insideScene, "Can't add an object that is already inside the scene tree.");
	m_insideScene = true;
	for (const std::shared_ptr<Object2D>& p : m_children)
	{
		p->onAddedToScene();
	}
}

_R2D_NAMESPACE_END_