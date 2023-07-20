#include "global.h"
#include "TileSet.h"

_R2D_NAMESPACE_START_

TileSet::TileSet()
{
}

void TileSet::createTile(TileID id)
{
  setTile(id, Tile());
}

void TileSet::setTile(TileID id, const Tile& tile)
{
  m_tiles.insert_or_assign(id, tile);
}

bool TileSet::hasTile(TileID id)
{
  return m_tiles.find(id) != m_tiles.end();
}

Tile& TileSet::getTile(TileID id)
{
  return m_tiles.at(id);
}

const Tile& TileSet::getTile(TileID id) const
{
  return m_tiles.at(id);
}

_R2D_NAMESPACE_END_