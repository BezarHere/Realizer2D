#pragma once
#include "../global.h"

_R2D_NAMESPACE_START_

struct Material
{
	Material();
	Material(const Material& copy);

	//sf::BlendMode blendMode = sf::BlendNone;
	sf::Shader* shader{ nullptr };
};

_R2D_NAMESPACE_END_