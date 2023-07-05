#pragma once
#include "global.h"

_R2D_NAMESPACE_START_

enum class ViewStretchMode : uint8_t
{
	Expand,
	KeepHorizontal,
	KeepVertical,
	Keep
};

enum class SpriteStretchMode : uint8_t
{
	// the sprite will expand to be the original size
	Expand,
	// the sprite will be drawen in a set size
	Confine
};

_R2D_NAMESPACE_END_