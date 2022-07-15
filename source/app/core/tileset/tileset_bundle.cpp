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

#include "tileset_bundle.hpp"

#include <utility>  // move

#include "misc/assert.hpp"
#include "misc/panic.hpp"

namespace tactile {

void TilesetBundle::attach_tileset(Shared<Tileset> tileset,
                                   const TileID    firstTileId,
                                   const bool      embedded)
{
  const auto id = tileset->get_uuid();
  mRefs.try_emplace(id, tileset, firstTileId, firstTileId + tileset->tile_count());

  auto& ref = lookup_in(mRefs, id);
  ref.set_embedded(embedded);

  for (auto tileId = ref.first_tile(); tileId <= ref.last_tile(); ++tileId) {
    mTileCache[tileId] = id;
  }
}

void TilesetBundle::attach_tileset(Shared<Tileset> tileset, const bool embedded)
{
  const auto count = tileset->tile_count();
  attach_tileset(std::move(tileset), mNextTileId, embedded);
  mNextTileId += count + 1;
}

void TilesetBundle::detach_tileset(const UUID& id)
{
  if (!mRefs.contains(id)) {
    throw TactileError {"Invalid tileset identifier!"};
  }

  TACTILE_ASSERT(mRefs.contains(id));
  const auto& ref = lookup_in(mRefs, id);

  // Removes all cached tile entries for the tileset that will be removed
  std::erase_if(mTileCache,
                [&](const auto& pair) { return ref.is_valid_tile(pair.first); });

  mRefs.erase(id);

  if (mActiveTileset == id) {
    mActiveTileset.reset();
  }

  if (!mActiveTileset && !mRefs.empty()) {
    mActiveTileset = mRefs.begin()->first;
  }

  TACTILE_ASSERT(!mRefs.contains(id));
}

void TilesetBundle::select_tileset(const UUID& id)
{
  if (mRefs.contains(id)) {
    mActiveTileset = id;
  }
  else {
    throw TactileError {"Invalid tileset identifier!"};
  }
}

auto TilesetBundle::has_tileset(const UUID& id) const -> bool
{
  return mRefs.contains(id);
}

auto TilesetBundle::is_valid_tile(const TileID id) const -> bool
{
  return mTileCache.contains(id);
}

auto TilesetBundle::find_tileset(const TileID tile) const -> Maybe<UUID>
{
  if (const auto iter = mTileCache.find(tile); iter != mTileCache.end()) [[likely]] {
    return iter->second;
  }
  else {
    return nothing;
  }
}

auto TilesetBundle::get_ref(const UUID& id) -> TilesetRef&
{
  return lookup_in(mRefs, id);
}

auto TilesetBundle::get_ref(const UUID& id) const -> const TilesetRef&
{
  return lookup_in(mRefs, id);
}

auto TilesetBundle::find_ref(const UUID& id) -> TilesetRef*
{
  if (const auto iter = mRefs.find(id); iter != mRefs.end()) {
    return &iter->second;
  }
  else {
    return nullptr;
  }
}

auto TilesetBundle::find_ref(const UUID& id) const -> const TilesetRef*
{
  if (const auto iter = mRefs.find(id); iter != mRefs.end()) {
    return &iter->second;
  }
  else {
    return nullptr;
  }
}

auto TilesetBundle::to_tile_index(const TileID id) const -> TileIndex
{
  if (const auto tilesetId = find_tileset(id)) {
    const auto& ref = get_ref(*tilesetId);
    return ref.to_index(id);
  }
  else {
    throw TactileError {"Invalid tile identifier!"};
  }
}

auto TilesetBundle::size() const -> usize
{
  return mRefs.size();
}

auto TilesetBundle::empty() const -> bool
{
  return mRefs.empty();
}

auto TilesetBundle::active_tileset_id() const -> const Maybe<UUID>&
{
  return mActiveTileset;
}

}  // namespace tactile
