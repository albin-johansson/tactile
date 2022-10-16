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

#include "core/tile/tileset_info.hpp"
#include "core/util/assoc.hpp"
#include "core/util/tiles.hpp"
#include "misc/panic.hpp"

namespace tactile {

Tileset::Tileset(TilesetInfo info, const UUID& id)
    : mContext {id}
    , mTextureId {info.texture_id}
    , mTextureSize {info.texture_size}
    , mTileSize {info.tile_size}
    , mRowCount {mTextureSize.y / mTileSize.y}
    , mColumnCount {mTextureSize.x / mTileSize.x}
    , mUvSize {Float2 {mTileSize} / Float2 {mTextureSize}}
    , mTexturePath {std::move(info.texture_path)}
{
  load_tiles();
}

void Tileset::load_tiles()
{
  const TileIndex count = tile_count();
  for (TileIndex index = 0; index < count; ++index) {
    const auto [row, col] = to_matrix_coords(index, mColumnCount);

    auto tile = std::make_shared<Tile>(index);
    const auto tile_id = tile->uuid();

    const Int2 pos {col * mTileSize.x, row * mTileSize.y};
    tile->set_source({pos, mTileSize});

    mTiles.try_emplace(tile_id, std::move(tile));
    mIdentifiers[index] = tile_id;
  }
}

void Tileset::accept(ContextVisitor& visitor) const
{
  visitor.visit(*this);
}

void Tileset::update()
{
  mAppearanceCache.clear();

  // TODO usually a lot of tiles to visit here, store which tiles that need updating?
  for (auto& [id, tile] : mTiles) {
    tile->update();
  }
}

auto Tileset::operator[](const TileIndex index) -> Tile&
{
  return *get_tile_ptr(index);
}

auto Tileset::operator[](const TileIndex index) const -> const Tile&
{
  const auto id = lookup_in(mIdentifiers, index);
  return *lookup_in(mTiles, id);
}

auto Tileset::get_tile_ptr(TileIndex index) -> const Shared<Tile>&
{
  const auto id = lookup_in(mIdentifiers, index);
  return lookup_in(mTiles, id);
}

auto Tileset::index_of(const TilePos& pos) const -> TileIndex
{
  if (is_valid(pos)) {
    return pos.row() * mColumnCount + pos.col();
  }
  else {
    throw TactileError {"Invalid tile position!"};
  }
}

auto Tileset::appearance_of(const TileIndex index) const -> TileIndex
{
  if (const auto iter = mAppearanceCache.find(index); iter != mAppearanceCache.end()) {
    return iter->second;
  }

  if (const auto iter = mIdentifiers.find(index); iter != mIdentifiers.end()) {
    const auto& id = iter->second;
    const auto& tile = lookup_in(mTiles, id);

    if (tile->is_animated()) {
      const auto appearance = tile->animation().current_frame().tile;
      mAppearanceCache[index] = appearance;
      return appearance;
    }
  }

  return index;
}

auto Tileset::ctx() -> ContextInfo&
{
  return mContext;
}

auto Tileset::ctx() const -> const ContextInfo&
{
  return mContext;
}

auto Tileset::uuid() const -> const UUID&
{
  return mContext.uuid();
}

auto Tileset::texture_id() const noexcept -> uint
{
  return mTextureId;
}

auto Tileset::texture_path() const -> const Path&
{
  return mTexturePath;
}

auto Tileset::texture_size() const noexcept -> const Int2&
{
  return mTextureSize;
}

auto Tileset::tile_size() const noexcept -> const Int2&
{
  return mTileSize;
}

auto Tileset::uv_size() const noexcept -> const Float2&
{
  return mUvSize;
}

auto Tileset::row_count() const noexcept -> int32
{
  return mRowCount;
}

auto Tileset::column_count() const noexcept -> int32
{
  return mColumnCount;
}

auto Tileset::tile_count() const noexcept -> int32
{
  return mRowCount * mColumnCount;
}

auto Tileset::is_valid(const TilePos& pos) const -> bool
{
  const auto row = pos.row();
  const auto col = pos.col();
  return row >= 0 && col >= 0 && row < mRowCount && col < mColumnCount;
}

}  // namespace tactile
