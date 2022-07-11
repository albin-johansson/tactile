/*
 * This source file is a part of the Tactile map editor.
 *
 * Copyright (C) 2022 Albin Johansson
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <https://www.gnu.org/licenses/>.
 */

#include "tileset_ref.hpp"

#include <utility>  // move

#include "misc/assert.hpp"
#include "misc/panic.hpp"

namespace tactile {

TilesetRef::TilesetRef(Shared<Tileset> tileset, TileID first, TileID last)
    : mTileset {std::move(tileset)}
    , mFirstTile {first}
    , mLastTile {last}
{
  if (!mTileset) {
    throw TactileError {"Invalid null tileset!"};
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

auto TilesetRef::get_tileset() -> const Shared<Tileset>&
{
  return mTileset;
}

auto TilesetRef::view_tileset() const -> const Tileset&
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