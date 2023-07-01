#pragma once
#include <deque>
#include <unordered_set>
#include "ObjectComponent2D.h"
#include "global.h"

_R2D_NAMESPACE_START_
typedef uint32_t ObjID_t;
typedef int8_t ZIndex_t;
class Engine;

class Object2D : public sf::Transformable
{
public:
	struct _FindID_SharedPtr
	{
		inline constexpr _FindID_SharedPtr(const ObjID_t p_id)
			: id{p_id} {}

		inline bool operator()(const std::shared_ptr<Object2D>& sptr) const
		{ return sptr ? sptr->getObjectID() == id : false; }
		const ObjID_t id;
	};

public:

	Object2D();
	~Object2D();

	virtual void update();
	
	virtual void draw(sf::RenderTarget& target, sf::RenderStates state) const;

	void addChild(const std::shared_ptr<Object2D>& child);
	void addChild(Object2D* child);

	bool hasChild(const std::shared_ptr<Object2D>& child) const;
	bool hasChild(const ObjID_t objid) const;

	bool removeChild(const std::shared_ptr<Object2D>& child);
	bool removeChild(const ObjID_t objid);

	Object2D* getChild(const ObjID_t objid) const;

	inline std::unordered_set<std::shared_ptr<Object2D>>& getChildren() { return m_children; }
	inline const std::unordered_set<std::shared_ptr<Object2D>>& getChildren() const { return m_children; }

	inline ObjID_t getObjectID() const { return m_objId; }
	inline void setObjectID(ObjID_t new_oid) { assert(m_objId == 0); m_objId = new_oid; }

	inline void setZIndex(ZIndex_t zindex) { m_zIndex = zindex; }
	inline ZIndex_t getZIndex() { return m_zIndex; }

	virtual void installComponent(ObjectComponent2D* component);
	virtual void removeComponent(ObjectComponent2D* component);
	// -- to check if a component is owned by this; check the getObject() == this_obj
	virtual bool hasComponent(ObjectComponent2D* component) const;
	virtual const std::unordered_set<std::shared_ptr<ObjectComponent2D>>& getComponents() const;
protected:
	// ---------------- signals ----------------

	virtual void onPreDelete();
	virtual void onRemovedFromScene();
	virtual void onAddedToScene();

public:
	Engine* engine;
private:
	ObjID_t m_objId = 0;
	//std::unordered_map<const char*, std::shared_ptr<ObjectComponent2D>> m_components;
	std::unordered_set<std::shared_ptr<ObjectComponent2D>> m_components;
	std::unordered_set<std::shared_ptr<Object2D>> m_children;
	Object2D* m_parent{ nullptr };
	ZIndex_t m_zIndex = 0;
	bool m_insideScene{ false };
};

_R2D_NAMESPACE_END_
