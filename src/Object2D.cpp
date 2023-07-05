#include "global.h"
#include "Object2D.h"
#include "Engine.h"

_R2D_NAMESPACE_START_

ObjectPath::ObjectPath(std::string ppath)
	: path(ppath), data(ObjectPath::GetData(ppath))
{
}

ObjectPath::ObjectPath(const ObjectPath& other)
	: path(other.path), data(other.data)
{
}

ObjectPath::PathData_t ObjectPath::GetData(const std::string& path)
{
	PathData_t data;
	size_t breakpoint_last{ 0 };
	while (breakpoint_last < path.size())
	{
		size_t find = path.find('/', breakpoint_last);
		if (find == -1)
			break;
		data.push_back(path.substr(breakpoint_last, find - breakpoint_last));
		breakpoint_last = find + 1;
	}
	PEEK(data.size());
	return data;
}

Object2D::Object2D()
	: engine{ Engine::Singleton() }
{
	assert_msg(engine, "can't create an Object2D without the engine starting first.");
	if (engine)engine->RegisterObject(this);
}

Object2D::Object2D(const std::string& name)
	: m_name(name), engine{ Engine::Singleton() }
{
	assert_msg(engine, "can't create an Object2D without the engine starting first.");
	if (engine)engine->RegisterObject(this);
}

Object2D::~Object2D()
{
	for (ObjectComponent2D* p : m_components)
	{
		delete p;
	}
	m_components.clear();
	for (const auto& kv : m_children)
	{
		delete kv.second;
	}
	engine->PopObject(this);
}

void Object2D::update(real_t delta)
{
	for (ObjectComponent2D* comp : m_components)
	{
		comp->update(delta);
	}
	for (const auto& kv : m_children)
	{
		kv.second->update(delta);
	}
}

void Object2D::draw(sf::RenderTarget& target, sf::RenderStates state) const
{
	state.transform.combine(getTransform());
	for (ObjectComponent2D* comp : m_components)
	{
		comp->draw(target, state);
	}
	for (const auto& kv : m_children)
	{
		kv.second->draw(target, state);
	}
}

void Object2D::kill()
{
	if (m_parent == nullptr)
	{
		SceneTree::DeleteObject(this);
	}
	else
	{
		delete this;
	}
}

void Object2D::addChild(Object2D* child)
{
	assert_msg(child->m_parent == nullptr, "Can't add an object to children when it has a parent");
	child->m_parent = this;
	if (m_insideScene)
	{
		child->_onAddedToScene();
	}
	m_children.insert_or_assign(child->getName(), child);
}


bool Object2D::hasChild(Object2D* child) const
{
	return std::find_if(m_children.begin(), m_children.end(), _Find_Value_In_Map<Object2D*>(child)) == m_children.end();
}

bool Object2D::hasChild(const std::string& name) const
{
	return m_children.find(name) != m_children.end();
}

bool Object2D::removeChild(Object2D* child)
{
	assert_msg(child->m_parent == this, "Can't remove an object from children when it's not a child");
	child->m_parent = nullptr;
	if (m_insideScene)
	{
		child->_onRemovedFromScene();
	}
	return (bool)m_children.erase(child->getName());
}

bool Object2D::removeChild(const std::string& name)
{
	return m_children.erase(name);
}


Object2D* Object2D::getChild(const std::string& name) const
{
	return (*m_children.find(name)).second;
}

Error Object2D::setName(const std::string& name)
{
	if (m_parent)
	{
		if (m_parent->hasChild(name))
			return ERR_NAME_ALREADY_EXISTS;

		m_parent->childRenamed(m_name, name);
	}
	m_name = name;
	return OK;
}

void Object2D::installComponent(ObjectComponent2D* component)
{
	// can't install a component owned by another object (or reistall an already installed component)
	assert(component->m_obj == nullptr);
	component->m_obj = this;
	m_components.insert(component);
}

void Object2D::removeComponent(ObjectComponent2D* component)
{
	// can't remove a component not owned by this
	assert(component->m_obj == this);
	component->m_obj = nullptr;
	m_components.erase(component);
}

bool Object2D::hasComponent(ObjectComponent2D* component) const
{
	for (ObjectComponent2D* p : m_components)
	{
		if (p == component)
			return true;
	}
	return false;
}

const std::unordered_set<ObjectComponent2D*>& Object2D::getComponents() const
{
	return m_components;
}

Vector2f_t Object2D::getGlobalPosition() const
{
	Vector2f_t position = getPosition();
	Object2D* parent = m_parent;
	while (parent)
	{
		position += parent->getPosition();
		parent = parent->getParent();
	}
	return position;
}

Vector2f_t Object2D::getGlobalRotation() const
{
	return Vector2f_t();
}

Vector2f_t Object2D::getGlobalScale() const
{
	return Vector2f_t();
}

Error Object2D::addToSceneTree()
{
	if (getParent())
	{
		_pr_error("can't add a low object (with a parent) to be a root object");
		return Error::FAILED;
	}
	SceneTree::AddObject(this);
	return Error::OK;
}

void Object2D::_onRemovedFromScene()
{
	assert_msg(m_insideScene, "Can't remove an already removed object from the scene tree.");
	m_insideScene = false;
	for (const auto& kv : m_children)
	{
		kv.second->_onRemovedFromScene();
	}
}

void Object2D::_onAddedToScene()
{
	assert_msg(!m_insideScene, "Can't add an object that is already inside the scene tree.");
	m_insideScene = true;
	for (const auto& kv : m_children)
	{
		kv.second->_onAddedToScene();
	}
}

void Object2D::childRenamed(const std::string& og_name, const std::string& new_name)
{
	Object2D* child = m_children.at(og_name);
	m_children.erase(og_name);
	m_children.insert_or_assign(new_name, child);
}

_R2D_NAMESPACE_END_