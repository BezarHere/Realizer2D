#pragma once
#include "global.h"
#include "Object2D.h"

_R2D_NAMESPACE_START_

// TODO: Create a scene data class

class SceneTree final
{
public:
	friend class Object2D;
	SceneTree();

	const std::unordered_set<std::shared_ptr<Object2D>>& getRootObjects() const { return m_rootObjects; }

	void addObject(std::shared_ptr<Object2D> object);
	void removeObject(std::shared_ptr<Object2D> object);
	// !FREES THE OBJECT POINTER!
	void deleteObject(std::shared_ptr<Object2D> object);

	// !WILL CALL deleteObject ON EVERY OBJECT!
	void clearScene();

private:
	ObjID_t m_objCounter = 0;
	std::unordered_map<ObjID_t, Object2D*> m_objsRegistery{};

	std::unordered_set<std::shared_ptr<Object2D>> m_rootObjects;
};

_R2D_NAMESPACE_END_