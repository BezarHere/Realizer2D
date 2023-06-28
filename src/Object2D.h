#pragma once
#include <deque>
#include <unordered_set>
#include "ObjectComponent2D.h"
#include "global.h"

_R2D_NAMESPACE_START_
typedef uint32_t ObjectID;

class Object2D : public sf::Transformable
{
public:
	Object2D();
	~Object2D();

	virtual void update();
	
	virtual void draw(sf::RenderTarget& target, sf::RenderStates state) const;

	inline ObjectID getObjectID() const { return m_objId; }
	inline void setObjectID(ObjectID new_oid) { assert(m_objId == 0); m_objId = new_oid; }

	virtual void installComponent(ObjectComponent2D* component);
	virtual void removeComponent(ObjectComponent2D* component);
	// -- to check if a component is owned by this; check the getObject() == this_obj
	virtual bool hasComponent(ObjectComponent2D* component) const;
	virtual const std::unordered_set<std::shared_ptr<ObjectComponent2D>>& getComponents() const;

private:
	ObjectID m_objId = 0;
	std::unordered_set<std::shared_ptr<ObjectComponent2D>> m_components;
	std::deque<std::shared_ptr<Object2D>> m_children;
	Object2D* m_parent{ nullptr };
};

_R2D_NAMESPACE_END_
