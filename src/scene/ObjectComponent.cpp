#include "global.h"
#include "ObjectComponent.h"

_R2D_NAMESPACE_START_

ObjectComponent::ObjectComponent()
{
}

uint32_t ObjectComponent::getSingleton() const
{
	return 0U;
}

void ObjectComponent::onOwnerVisiblityChanged()
{
}

void ObjectComponent::onOwnerDetached()
{
}

void ObjectComponent::onOwnerAtached()
{
}

void ObjectComponent::entredScene()
{
}

void ObjectComponent::exitedScene()
{
}

void ObjectComponent::setActive(bool active)
{
	m_active = active;
}

_R2D_NAMESPACE_END_

