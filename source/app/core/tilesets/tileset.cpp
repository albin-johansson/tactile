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

#include "tileset.hpp"

#include <utility>  // move

#include "core/contexts/context_visitor.hpp"
#include "core/tilesets/tileset_info.hpp"
#include "core/utils/tiles.hpp"
#include "misc/panic.hpp"

namespace tactile::core {

Tileset::Tileset(const UUID& id, const TilesetInfo& info)
    : mContext{id}
    , mTextureId{info.texture_id}
    , mTextureSize{info.texture_size}
    , mTileSize{info.tile_size}
    , mRowCount{mTextureSize.y / mTileSize.y}
    , mColumnCount{mTextureSize.x / mTileSize.x}
    , mUvSize{Vector2f{mTileSize} / Vector2f{mTextureSize}}
    , mTexturePath{std::move(info.texture_path)}
{
  load_tiles();
}

Tileset::Tileset(const TilesetInfo& info) : Tileset{make_uuid(), info} {}

void Tileset::load_tiles()
{
  const TileIndex count = tile_count();
  for (TileIndex index = 0; index < count; ++index) {
    const auto [row, col] = to_matrix_coords(index, mColumnCount);

    Tile       tile{index};
    const auto tileId = tile.get_uuid();

    const Vector2i pos{col * mTileSize.x, row * mTileSize.y};
    tile.set_source({pos, mTileSize});

    mMetaTiles.try_emplace(tileId, std::move(tile));
    mIdentifiers[index] = tileId;
  }
}

void Tileset::update()
{
  mAppearanceCache.clear();

  for (auto& [id, tile] : mMetaTiles) {
    tile.update();
  }
}

void Tileset::accept(IContextVisitor& visitor) const
{
  visitor.visit(*this);
}

void Tileset::set_name(std::string name)
{
  mContext.set_name(std::move(name));
}

auto Tileset::operator[](const TileIndex index) -> Tile&
{
  const auto id = lookup_in(mIdentifiers, index);
  return lookup_in(mMetaTiles, id);
}

auto Tileset::operator[](const TileIndex index) const -> const Tile&
{
  const auto id = lookup_in(mIdentifiers, index);
  return lookup_in(mMetaTiles, id);
}

auto Tileset::index_of(const TilePos& pos) const -> TileIndex
{
  const auto row = pos.row();
  const auto col = pos.col();

  if (row >= 0 && col >= 0 && row < mRowCount && col < mColumnCount) {
    return row * mColumnCount + col;
  }
  else {
    throw TactileError{"Invalid tile position!"};
  }
}

auto Tileset::appearance_of(const TileIndex index) const -> TileIndex
{
  if (const auto iter = mAppearanceCache.find(index); iter != mAppearanceCache.end()) {
    return iter->second;
  }

  if (const auto iter = mIdentifiers.find(index); iter != mIdentifiers.end()) {
    const auto& id = iter->second;
    const auto& tile = lookup_in(mMetaTiles, id);

    if (tile.is_animated()) {
      const auto appearance = tile.get_animation().current_tile();
      mAppearanceCache[index] = appearance;
      return appearance;
    }
  }

  return index;
}

auto Tileset::get_props() -> PropertyBundle&
{
  return mContext.get_props();
}

auto Tileset::get_props() const -> const PropertyBundle&
{
  return mContext.get_props();
}

auto Tileset::get_comps() -> ComponentBundle&
{
  return mContext.get_comps();
}

auto Tileset::get_comps() const -> const ComponentBundle&
{
  return mContext.get_comps();
}

auto Tileset::get_uuid() const -> const UUID&
{
  return mContext.get_uuid();
}

auto Tileset::get_name() const -> const std::string&
{
  return mContext.get_name();
}

auto Tileset::is_valid(const TileIndex index) const -> bool
{
  return index >= 0 && index < tile_count();
}

}  // namespace tactile::core
