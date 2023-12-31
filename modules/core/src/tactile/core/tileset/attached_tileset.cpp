// Copyright (C) 2023 Albin Johansson (GNU General Public License v3.0)

#include "tactile/core/tileset/attached_tileset.hpp"

#include <utility>  // move

#include "tactile/foundation/debug/assert.hpp"
#include "tactile/foundation/debug/validation.hpp"

namespace tactile {

AttachedTileset::AttachedTileset(Shared<Tileset> tileset, const TileID first_tile_id)
  : mTileset {require_not_null(std::move(tileset), "null tileset")},
    mFirstTileId {first_tile_id},
    mLastTileId {static_cast<TileID::value_type>(first_tile_id.value +
                                                 mTileset->last_tile_index().value)}
{}

auto AttachedTileset::to_index(const TileID tile_id) const -> Maybe<TileIndex>
{
  if (has_tile(tile_id)) {
    return TileIndex {tile_id.value - mFirstTileId.value};
  }

  return kNothing;
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

auto AttachedTileset::tileset() -> Tileset&
{
  TACTILE_ASSERT(mTileset != nullptr);
  return *mTileset;
}

auto AttachedTileset::tileset() const -> const Tileset&
{
  TACTILE_ASSERT(mTileset != nullptr);
  return *mTileset;
}

auto AttachedTileset::viewport() -> DocumentViewport&
{
  return mViewport;
}

auto AttachedTileset::viewport() const -> const DocumentViewport&
{
  return mViewport;
}

}  // namespace tactile
