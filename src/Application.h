#pragma once
#include "global.h"
#include "core/StretchMode.h"

_R2D_NAMESPACE_START_

struct ApplicationConfig
{
	std::string title { "Title" };
	// the window size at program init
	sf::Vector2u windowSize {512u, 400u};
	ViewStretchMode stretchMode { ViewStretchMode::Expand };
};

_R2D_NAMESPACE_END_