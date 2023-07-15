#pragma once
#include "global.h"
#include "Object.h"

_R2D_NAMESPACE_START_

// TODO: Create a scene data class
class Engine;
class SceneTree final
{
	friend class Engine;
public:
	SceneTree() = delete;

	static inline const std::unordered_set<Object2D*>& GetRootObjects() { return s_rootObjects; }

	static void AddObject(Object2D* object);
	static void RemoveObject(Object2D* object);
	// Removes the object from the scene tree and frees it
	static void DeleteObject(Object2D* object);

	static bool isRootObject(Object2D* object);
	static bool isQueuedForDeletion(Object2D* object);

	// !WILL CALL deleteObject ON EVERY OBJECT!
	static void ClearScene();
private:
	
	static void FlushDeletionQueue();

private:
	static inline std::unordered_set<Object2D*> s_rootObjects;
	static inline std::unordered_set<Object2D*> s_deleteQueue;
};

_R2D_NAMESPACE_END_