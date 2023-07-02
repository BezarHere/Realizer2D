#pragma once
#include "global.h"

_R2D_NAMESPACE_START_

enum ViewStretchMode : uint8_t
{
	Expand,
	KeepHorizontal,
	KeepVertical,
	Keep
};

struct ApplicationConfig
{
	std::string title { "Title" };
	// the window size at program init
	sf::Vector2u windowSize {512u, 400u};
	ViewStretchMode stretchMode { ViewStretchMode::Expand };
};

_R2D_NAMESPACE_END_