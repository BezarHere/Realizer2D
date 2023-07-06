#pragma once
#include <deque>
#include <unordered_set>
#include "ObjectComponent2D.h"
#include "global.h"
#include "core/Errors.h"

_R2D_NAMESPACE_START_
typedef uint32_t ObjID_t;
typedef int8_t ZIndex_t;

class Engine;
class SceneTree;

struct ObjectPath
{
	typedef std::vector<std::string> PathData_t;
	friend std::ostream& operator<<(std::ostream& stream, const ObjectPath& obj);

	ObjectPath(std::string path);
	ObjectPath(const ObjectPath& other);

	static PathData_t GetData(const std::string& path);

	const std::string path;
	const PathData_t data;
};

inline std::ostream& operator<<(std::ostream& stream, const ObjectPath& obj)
{
	stream << '@' << obj.path;
	return stream;
}

class Object2D : public sf::Transformable
{
	friend class SceneTree;
	friend class Engine;
public:
	struct _FindID_By_Ptr
	{
		inline constexpr _FindID_By_Ptr(const ObjID_t p_id)
			: id{p_id} {}

		inline bool operator()(const Object2D* sptr) const
		{ return sptr ? sptr->getObjectID() == id : false; }
		const ObjID_t id;
	};

public:
	Object2D();
	Object2D(const std::string& name);
	~Object2D();

	void addChild(Object2D* child);

	bool hasChild(Object2D* child) const;
	bool hasChild(const std::string& name) const;

	bool removeChild(Object2D* child);
	bool removeChild(const std::string& name);

	Object2D* getChild(const std::string& objid) const;

	inline std::unordered_map<std::string, Object2D*>& getChildren() { return m_children; }
	inline const std::unordered_map<std::string, Object2D*>& getChildren() const { return m_children; }

	inline const std::string& getName() const { return m_name; }
	Error setName(const std::string& name);
	inline Error rename(const std::string& name) { setName(name); }

	inline ObjID_t getObjectID() const { return m_objId; }
	inline void setObjectID(ObjID_t new_oid) { assert(m_objId == 0); m_objId = new_oid; }

	// returns first component of with typehash
	ObjectComponent2D* getComponent(const size_t typehash) const;

	void setZIndex(ZIndex_t zindex);
	inline ZIndex_t getZIndex() { return m_zIndex; }

	virtual void installComponent(ObjectComponent2D* component);
	virtual void removeComponent(ObjectComponent2D* component);
	// -- to check if a component is owned by this; check the getObject() == this_obj
	virtual bool hasComponent(ObjectComponent2D* component) const;
	virtual const std::unordered_set<ObjectComponent2D*>& getComponents() const;

	Vector2 getGlobalPosition() const;
	Vector2 getGlobalRotation() const;
	Vector2 getGlobalScale() const;

	Error addToSceneTree();

	inline Object2D* getParent() const { return m_parent; }

	virtual void update(real_t delta);
	virtual void draw(sf::RenderTarget& target, sf::RenderStates state) const;

	void kill();

	inline bool isQueuedForDeletion() const { return m_queuedForDeletion; }

	virtual void _onRemovedFromScene();
	virtual void _onAddedToScene();

public:

private:
	// called before renaming, check for name collisions before calling
	void childRenamed(const std::string& og_name, const std::string& new_name);
private:
	Engine* engine;
	std::string m_name;
	ObjID_t m_objId = 0;
	//std::unordered_map<const char*, std::shared_ptr<ObjectComponent2D>> m_components;
	std::unordered_set<ObjectComponent2D*> m_components;
	std::unordered_map<std::string, Object2D*> m_children;
	Object2D* m_parent{ nullptr };
	ZIndex_t m_zIndex = 0;
	bool m_insideScene{ false };
	bool m_queuedForDeletion{ false };
};

_R2D_NAMESPACE_END_
