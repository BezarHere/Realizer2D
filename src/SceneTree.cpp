#include "global.h"
#include "SceneTree.h"

_R2D_NAMESPACE_START_

SceneTree::SceneTree()
	: m_rootObjects()
{
}


void SceneTree::addObject(std::shared_ptr<Object2D> object)
{
}

void SceneTree::removeObject(std::shared_ptr<Object2D> object)
{
}

void SceneTree::deleteObject(std::shared_ptr<Object2D> object)
{
	removeObject(object);
}

void SceneTree::clearScene()
{
}

_R2D_NAMESPACE_END_