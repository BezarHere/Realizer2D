#include "global.h"
#include "Object.h"
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
{
	Engine::RegisterObject(this);
}

Object2D::Object2D(const std::string& name)
	: m_name(name)
{
	Engine::RegisterObject(this);
}

Object2D::~Object2D()
{
	for (ObjectComponent* p : m_components)
	{
		delete p;
	}
	m_components.clear();
	for (const auto& kv : m_children)
	{
		delete kv.second;
	}
	Engine::PopObject(this);
}

void Object2D::update(real_t delta)
{
	for (ObjectComponent* comp : m_components)
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
	for (ObjectComponent* comp : m_components)
	{
		comp->draw(target, state);
	}
	for (const auto& kv : m_children)
	{
		kv.second->draw(target, state);
	}
}


// TODO: Implement more rubost kill function
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

bool Object2D::isVisible() const
{
	return m_visible;
}

bool Object2D::isBranchVisible() const
{
	return m_visible && m_parent ? m_branchVisible : true;
}

void Object2D::setVisible(bool visible)
{
	if (visible == m_visible)
		return;
	m_visible = visible;

	propgateVisiblityChangeCallback();
}

void Object2D::show()
{
	setVisible(true);
}

void Object2D::hide()
{
	setVisible(false);
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
			return Error::NameAlreadyExists;

		m_parent->childRenamed(m_name, name);
	}
	m_name = name;
	return Error::Ok;
}

ObjectComponent* Object2D::getComponent(const size_t index) const
{
	return m_components[index];
}

void Object2D::setZIndex(ZIndex_t zindex)
{
	m_zIndex = zindex;
}

Error Object2D::addComponent(ObjectComponent* component)
{
	// can't install a component owned by another object (or reistall an already installed component)
	assert(component->m_owner == nullptr);

	// already has a singleton or this isn't a singleton
	if (hasComponentSingleton(component->getSingleton()))
	{
		return Error::AlreadyExists;
	}
	else
	{
		m_singletonComponentLookupTable.insert(component->getSingleton());
	}

	component->m_owner = this;
	component->onOwnerAtached();
	m_components.push_back(component);
	return Error::Ok;
}

void Object2D::removeComponent(ObjectComponent* component)
{
	// can't remove a component not owned by this
	assert(component->m_owner == this);
	removeComponent(component, true);
}

std::vector<ObjectComponent*>::const_iterator Object2D::findComponent(ObjectComponent* component) const
{
	for (auto i = m_components.begin(); i != m_components.end(); i++)
	{
		if (*i == component)
			return i;
	}
	// can't find it :(
	return m_components.end();
}

bool Object2D::hasComponent(ObjectComponent* component) const
{
	for (ObjectComponent* p : m_components)
	{
		if (p == component)
			return true;
	}
	return false;
}

const std::vector<ObjectComponent*>& Object2D::getComponents() const
{
	return m_components;
}

Transform2D Object2D::getGlobalTransform() const
{
	Transform2D transform = getTransform();
	if (m_parent)
	{
		transform *= m_parent->getGlobalTransform();
	}
	return transform;
}

Vector2 Object2D::getGlobalPosition() const
{
	Vector2 position = getPosition();
	if (m_parent)
	{
		position += m_parent->getGlobalPosition();
	}
	return position;
}

real_t Object2D::getGlobalRotation() const
{
	// no good
	return ((Vector2*)getGlobalTransform().getMatrix())->angle();
}

Vector2 Object2D::getGlobalScale() const
{
	Vector2 scale = getScale();
	if (m_parent)
	{
		scale *= m_parent->getGlobalScale();
	}
	return scale;
}

Error Object2D::addToSceneTree()
{
	if (getParent())
	{
		_r2d_error("can't add a low object (with a parent) to be a root object");
		return Error::Failed;
	}
	SceneTree::AddObject(this);
	return Error::Ok;
}

bool Object2D::isInsideScene() const
{
	return m_insideScene;
}

bool Object2D::hasComponentSingleton(uint8_t singleton_id) const
{
	// TODO: make this faster...
	return singleton_id ?
		m_singletonComponentLookupTable.find(singleton_id) != m_singletonComponentLookupTable.end() : false;
}

void Object2D::regenerateComponentsSingletonTable()
{
	m_singletonComponentLookupTable.clear();
	for (ObjectComponent* comp : m_components)
	{
		if (comp->getSingleton())
		{
			if (hasComponentSingleton(comp->getSingleton()))
			{
				// FIXME: no more info? what's the type of the component?
				_r2d_error("found a bugged singleton component with sig-id: " + std::to_string(comp->getSingleton()) + ", deleting the component.");
				removeComponent(comp, false);
			}
			else
			{
				m_singletonComponentLookupTable.insert(comp->getSingleton());
			}
		}
	}
}

void Object2D::removeComponent(ObjectComponent* component, bool update_singleton)
{
	component->onOwnerDetached();
	component->m_owner = nullptr;

	if (update_singleton && component->getSingleton())
		m_singletonComponentLookupTable.erase(component->getSingleton());

	m_components.erase(findComponent(component));
}

void Object2D::updateBranchVisiblty()
{
	bool old_bv = m_branchVisible;
	m_branchVisible = m_parent ? m_parent->isBranchVisible() : true;
	if (old_bv != m_branchVisible)
		propgateVisiblityChangeCallback();
}

void Object2D::propgateVisiblityChangeCallback()
{
	for (ObjectComponent* comp : m_components)
	{
		comp->onOwnerVisiblityChanged();
	}

	for (const auto& kv : m_children)
	{
		kv.second->updateBranchVisiblty();
	}
}

void Object2D::doUpdate(real_t delta)
{
	update(delta);
}

void Object2D::_onRemovedFromScene()
{
	assert_msg(m_insideScene, "Can't remove an already removed object from the scene tree.");
	m_insideScene = false;
	for (const auto& kv : m_children)
	{
		kv.second->_onRemovedFromScene();
	}
	for (auto comp : m_components)
	{
		comp->exitedScene();
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
	for (auto comp : m_components)
	{
		comp->entredScene();
	}
}

void Object2D::childRenamed(const std::string& og_name, const std::string& new_name)
{
	Object2D* child = m_children.at(og_name);
	m_children.erase(og_name);
	m_children.insert_or_assign(new_name, child);
}

_R2D_NAMESPACE_END_