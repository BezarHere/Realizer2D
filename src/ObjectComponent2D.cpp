#include "global.h"
#include "ObjectComponent2D.h"

_R2D_NAMESPACE_START_

ObjectComponent2D::ObjectComponent2D()
	: m_active{ false }, m_obj{ nullptr }, unique_id{ 0 }
{
}

ObjectComponent2D::ObjectComponent2D(const uint64_t uid)
	: m_active{ false }, m_obj{ nullptr }, unique_id{ uid }
{
}

_R2D_NAMESPACE_END_

