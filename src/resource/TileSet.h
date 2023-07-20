#pragma once
#include "global.h"

_R2D_NAMESPACE_START_

typedef uint32_t TileID;

struct Tile
{
	inline Tile() {}

	sf::Texture texture;
};

class TileSet
{
public:
	TileSet();

	void createTile(TileID id);
	void setTile(TileID id, const Tile& tile);

	bool hasTile(TileID id);

	Tile& getTile(TileID id);
	const Tile& getTile(TileID id) const;


private:
	std::unordered_map<TileID, Tile> m_tiles{};
};

_R2D_NAMESPACE_END_