// Copyright (C) 2024 Albin Johansson (GNU General Public License v3.0)

#include "tileset_ref.hpp"

#include <utility>  // move

#include "tactile/core/debug/assert.hpp"
#include "tactile/core/debug/exception.hpp"

namespace tactile {

TilesetRef::TilesetRef(Shared<Tileset> tileset, const TileID first, const TileID last)
    : mTileset {std::move(tileset)},
      mFirstTile {first},
      mLastTile {last}
{
  if (!mTileset) {
    throw Exception {"Invalid null tileset!"};
  }
}

void TilesetRef::update()
{
  mTileset->update();
}

void TilesetRef::set_selection(const Maybe<Region>& selection)
{
  mSelection = selection;
}

void TilesetRef::set_embedded(const bool embedded)
{
  mEmbedded = embedded;
}

auto TilesetRef::get_selection() const -> const Maybe<Region>&
{
  return mSelection;
}

auto TilesetRef::get_tileset_ptr() -> const Shared<Tileset>&
{
  return mTileset;
}

auto TilesetRef::get_tileset() const -> const Tileset&
{
  return *mTileset;
}

auto TilesetRef::to_index(const TileID id) const -> TileIndex
{
  TACTILE_ASSERT(id >= mFirstTile);
  TACTILE_ASSERT(id <= mLastTile);
  return id - mFirstTile;
}

auto TilesetRef::is_valid_tile(const TileID id) const -> bool
{
  return id >= mFirstTile && id <= mLastTile;
}

auto TilesetRef::is_single_tile_selected() const -> bool
{
  if (mSelection) {
    return mSelection->end - mSelection->begin == TilePos {1, 1};
  }
  else {
    return false;
  }
}

}  // namespace tactile
