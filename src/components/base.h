#pragma once
#include "global.h"
#include "scene/ObjectComponent.h"

#define _R2D_COMP_NS_START_ _R2D_NAMESPACE_START_ namespace components {
#define _R2D_COMP_NS_END_ }_R2D_NAMESPACE_END_

#define _R2D_register_comp(comp) _R2D_NAMESPACE_START_ typedef components::comp comp; _R2D_NAMESPACE_END_
