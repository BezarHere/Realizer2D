#include "global.h"
#include "Material.h"

_R2D_NAMESPACE_START_

Material::Material()
{
}

// TODO: Fix shader coping
Material::Material(const Material& copy)
{
	//shader->CurrentTexture = copy.shader->CurrentTexture;
}

_R2D_NAMESPACE_END_