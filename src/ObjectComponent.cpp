#include "global.h"
#include "ObjectComponent.h"

_R2D_NAMESPACE_START_

ObjectComponent::ObjectComponent()
	: m_active{ false }, m_obj{ nullptr }, unique_id{ 0 }
{
}

ObjectComponent::ObjectComponent(const uint64_t uid)
	: m_active{ false }, m_obj{ nullptr }, unique_id{ uid }
{
}

_R2D_NAMESPACE_END_

