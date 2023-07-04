#include "global.h"
#include "SceneTree.h"

_R2D_NAMESPACE_START_


void SceneTree::AddObject(Object2D* object)
{
	PEEK(object->m_parent);
	object->_onAddedToScene();
	s_rootObjects.insert(object);
}

void SceneTree::RemoveObject(Object2D* object)
{
	object->_onRemovedFromScene();
	s_rootObjects.erase(object);
}

void SceneTree::DeleteObject(Object2D* object)
{
	object->m_queuedForDeletion = true;
	s_deleteQueue.insert(object);
	//RemoveObject(object);
	//delete object;
}

bool SceneTree::isRootObject(Object2D* object)
{
	return s_rootObjects.find(object) != s_rootObjects.end();
}

bool SceneTree::isQueuedForDeletion(Object2D* object)
{
	return s_deleteQueue.find(object) != s_deleteQueue.end();
}

void SceneTree::ClearScene()
{
	for (Object2D *p : s_rootObjects)
	{
		DeleteObject(p);
	}
	s_rootObjects.clear();
}

void SceneTree::FlushDeletionQueue()
{
	for (Object2D* obj : s_deleteQueue)
	{
		RemoveObject(obj);
		delete obj;
	}
	s_deleteQueue.clear();
}

_R2D_NAMESPACE_END_