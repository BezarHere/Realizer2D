#pragma once
#include <deque>
#include <unordered_set>
#include "ObjectComponent.h"
#include "global.h"
#include "core/Errors.h"

_R2D_NAMESPACE_START_
typedef uint32_t ObjID_t;
typedef int16_t ZIndex_t;

enum class Margin
{
	Top, Left, Bottom, Right
};

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

	template <typename _CompType>
	inline _CompType* getComponent(const uint32_t index) const
	{
		return (_CompType*)(m_components[index]);
	}
	/*inline ObjectComponent* getComponent(const uint32_t index) const
	{
		return m_components[index];
	}*/

	void setZIndex(ZIndex_t zindex);
	ZIndex_t getZIndex() const;
	ZIndex_t getAbsoluteZIndex() const;

	virtual Error addComponent(ObjectComponent* component);
	virtual void removeComponent(ObjectComponent* component);
	virtual std::vector<ObjectComponent*>::const_iterator findComponent(ObjectComponent* component) const;
	// -- to check if a component is owned by this; check the getOwner() == this_obj
	virtual bool hasComponent(ObjectComponent* component) const;
	virtual const std::vector<ObjectComponent*>& getComponents() const;

	Transform2D getGlobalTransform() const;
	Vector2 getGlobalPosition() const;
	real_t getGlobalRotation() const;
	Vector2 getGlobalScale() const;

	Vector2 toGlobal(const Vector2 &position) const;
	Vector2 toLocal(const Vector2 &position) const;

	Error addToSceneTree();

	inline Object2D* getParent() const { return m_parent; }

	virtual void update(real_t delta);
	virtual void draw(sf::RenderTarget& target, sf::RenderStates state) const;

	void kill();

	inline bool isQueuedForDeletion() const { return m_queuedForDeletion; }

	bool isVisible() const;

	// if the object or any parent of it is invisible, this will return false otherwise true
	bool isBranchVisible() const;

	void setVisible(bool visible);
	void show();
	void hide();
	
	// if true: this will be drawn like every other game object
	// else: this will be drawn directly on screen like UI
	void setFixedDraw(bool fixed_draw);

	// will the pbject drawing be effected by the camera, if this is false the object will draw like UI
	// all children will have the same property value, so this only works on root objects
	bool getFixedDraw() const;

	// exciacly like getFixedDraw()
	bool isFixedDrawen() const;

	// if this object is in the scene tree
	// only objects in the scene tree get updated/drawn
	// if the object is outside the tree, it's more or less in an idle state
	bool isInsideScene() const;

	// only works with components that has isSingleton() == true
	bool hasComponentSingleton(uint32_t singleton_id) const;

	// call this if you find some weired bugs with adding singleton components (and also report the bug!)
	void regenerateComponentsSingletonTable();

private:
	void removeComponent(ObjectComponent* component, bool update_singleton);

	void updateBranchVisiblty();
	void propgateVisiblityChangeCallback();

	void doUpdate(real_t delta);

	virtual void _onRemovedFromScene();
	virtual void _onAddedToScene();

private:
	// called before renaming, check for name collisions before calling
	void childRenamed(const std::string& og_name, const std::string& new_name);
private:
	std::string m_name;
	ObjID_t m_objId = 0;
	//std::unordered_map<const char*, std::shared_ptr<ObjectComponent>> m_components;
	std::vector<ObjectComponent*> m_components;
	std::unordered_set<uint32_t> m_singletonComponentLookupTable;
	std::unordered_map<std::string, Object2D*> m_children;
	Object2D* m_parent{ nullptr };
	ZIndex_t m_zIndex{ 0 };
	// position (precentage) on screen where this will be anchored, only used if the object is fixed-drawn
	real_t m_screenAnchors[4]{ 0.0f, 0.0f, 1.0, 1.0f };

	Vector2 m_screenAnchorOffset{ 0.0f, 0.0f };
	Vector2 m_screenAnchorScale{ 1.0f, 1.0f };

	// if false, it will be drawn in world, dynamicly affected by camera
	// if true, it will be drawn like a user interface one the screen directly
	// only works on root objects, lower objects have their set by their parents
	bool m_fixedDraw{ false };

	bool m_visible{ true };
	bool m_branchVisible{ true };
	bool m_insideScene{ false };
	bool m_queuedForDeletion{ false };
	bool m_relativeZIndex{ true };
};

_R2D_NAMESPACE_END_
