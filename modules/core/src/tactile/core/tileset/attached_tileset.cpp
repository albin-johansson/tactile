// Copyright (C) 2023 Albin Johansson (GNU General Public License v3.0)

#include "tactile/core/tileset/attached_tileset.hpp"

#include <utility>  // move

#include "tactile/foundation/debug/assert.hpp"
#include "tactile/foundation/debug/exception.hpp"

namespace tactile {

AttachedTileset::AttachedTileset(Shared<Tileset> tileset, const TileID first_tile_id)
  : mTileset {tileset ? std::move(tileset) : throw Exception {"Null tileset"}},
    mFirstTileId {first_tile_id},
    mLastTileId {first_tile_id.value + (mTileset->tile_count() - 1)}
{}

auto AttachedTileset::to_index(const TileID tile_id) const -> TileIndex
{
  TACTILE_ASSERT(has_tile(tile_id));
  return TileIndex {tile_id.value - mFirstTileId.value};
}

auto AttachedTileset::has_tile(const TileID tile_id) const -> bool
{
  return tile_id >= mFirstTileId && tile_id <= mLastTileId;
}

auto AttachedTileset::get_first_tile_id() const -> TileID
{
  return mFirstTileId;
}

auto AttachedTileset::get_last_tile_id() const -> TileID
{
  return mLastTileId;
}

auto AttachedTileset::get_tileset() -> Tileset&
{
  return *mTileset;
}

auto AttachedTileset::get_tileset() const -> const Tileset&
{
  return *mTileset;
}

}  // namespace tactile
