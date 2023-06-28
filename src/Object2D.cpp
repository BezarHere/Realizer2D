#include "global.h"
#include "Object2D.h"
#include "Engine.h"

_R2D_NAMESPACE_START_

Object2D::Object2D()
{
	Engine::Singleton()->RegisterObject(this);
}

Object2D::~Object2D()
{
	Engine::Singleton()->PopObject(this);
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

void Object2D::installComponent(ObjectComponent2D* component)
{
	// can't install a component owned by another object (or reistall an already installed component)
	assert(component->m_obj == nullptr);
	component->m_obj = this;
	m_components.insert(std::unique_ptr<ObjectComponent2D>(component));
}

void Object2D::removeComponent(ObjectComponent2D* component)
{
	// can't remove a component not owned by this
	assert(component->m_obj == this);

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

_R2D_NAMESPACE_END_