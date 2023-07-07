#include "global.h"
#include "ObjectComponent.h"

_R2D_NAMESPACE_START_

ObjectComponent::ObjectComponent()
{
}

void ObjectComponent::ownerVisiblityChangeCallback()
{
}

void ObjectComponent::ownerDetachedCallback()
{
}

void ObjectComponent::ownerAtachedCallback()
{
}

void ObjectComponent::setActive(bool active)
{
	m_active = active;
}

_R2D_NAMESPACE_END_

