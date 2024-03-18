// Copyright (C) 2024 Albin Johansson (GNU General Public License v3.0)

#include "tileset_bundle.hpp"

#include <utility>  // move

#include "tactile/core/debug/assert.hpp"
#include "tactile/core/debug/exception.hpp"
#include "tactile/core/util/lookup.hpp"

namespace tactile {

void TilesetBundle::attach_tileset(Shared<Tileset> tileset,
                                   const TileID first_tile_id,
                                   const bool embedded)
{
  const auto id = tileset->get_uuid();
  const auto last_tile_id = first_tile_id + tileset->tile_count();
  mRefs.try_emplace(id, std::move(tileset), first_tile_id, last_tile_id);

  auto& ref = lookup_in(mRefs, id);
  ref.set_embedded(embedded);

  for (auto tile_id = ref.get_first_tile(); tile_id <= ref.get_last_tile(); ++tile_id) {
    mTileCache[tile_id] = id;
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
  if (!has_tileset(id)) {
    throw Exception {"Invalid tileset identifier!"};
  }

  const auto& ref = lookup_in(mRefs, id);

  // Removes all cached tile entries for the tileset that will be removed
  erase_if(mTileCache, [&](const auto& pair) { return ref.is_valid_tile(pair.first); });

  mRefs.erase(id);

  if (mActiveTileset == id) {
    mActiveTileset.reset();
  }

  if (!mActiveTileset && !mRefs.empty()) {
    mActiveTileset = mRefs.begin()->first;
  }

  TACTILE_ASSERT(!has_tileset(id));
}

void TilesetBundle::select_tileset(const UUID& id)
{
  if (has_tileset(id)) {
    mActiveTileset = id;
  }
  else {
    throw Exception {"Invalid tileset identifier!"};
  }
}

void TilesetBundle::set_next_tile_id(const TileID next_id)
{
  mNextTileId = next_id;
}

auto TilesetBundle::has_tileset(const UUID& id) const -> bool
{
  return mRefs.find(id) != mRefs.end();
}

auto TilesetBundle::is_valid_tile(const TileID id) const -> bool
{
  return mTileCache.find(id) != mTileCache.end();
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

auto TilesetBundle::get_tileset_ref(const UUID& id) -> TilesetRef&
{
  return lookup_in(mRefs, id);
}

auto TilesetBundle::get_tileset_ref(const UUID& id) const -> const TilesetRef&
{
  return lookup_in(mRefs, id);
}

auto TilesetBundle::find_tileset_ref(const UUID& id) -> TilesetRef*
{
  if (const auto iter = mRefs.find(id); iter != mRefs.end()) {
    return &iter->second;
  }
  else {
    return nullptr;
  }
}

auto TilesetBundle::find_tileset_ref(const UUID& id) const -> const TilesetRef*
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
  if (const auto tileset_id = find_tileset(id)) {
    const auto& ref = get_tileset_ref(*tileset_id);
    return ref.to_index(id);
  }
  else {
    throw Exception {"Invalid tile identifier!"};
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

auto TilesetBundle::get_active_tileset_id() const -> const Maybe<UUID>&
{
  return mActiveTileset;
}

}  // namespace tactile
