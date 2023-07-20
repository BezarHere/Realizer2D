#pragma once
#include "base.h"
#include "resource/TileSet.h"

_R2D_COMP_NS_START_

class TimeMap : public ObjectComponent
{
private:
	Vector2 m_tileSize{ 16.0f, 16.0f };
	TileSet m_tileset{};
};

_R2D_COMP_NS_END_