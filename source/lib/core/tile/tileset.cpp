/*
 * This source file is a part of the Tactile map editor.
 *
 * Copyright (C) 2023 Albin Johansson
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

#include "common/debug/assert.hpp"
#include "common/util/assoc.hpp"
#include "core/tile/tile.hpp"
#include "core/tile/tileset_info.hpp"
#include "tactile/core/debug/error.hpp"
#include "tile_matrix.hpp"

namespace tactile {

struct Tileset::Data final {
  ContextInfo context;
  Shared<Texture> texture;
  Int2 tile_size {};
  int32 row_count {};
  int32 column_count {};
  Float2 uv_size {};
  HashMap<TileIndex, UUID> identifiers;
  HashMap<UUID, Shared<Tile>> tiles;
  Maybe<TileIndex> selected_tile;

  /// Optimization to avoid iterating all tiles in the update function.
  // TODO Vec<UUID> mAnimatedTiles;

  /// A cache of the tiles that should be rendered when a tile is encountered
  mutable HashMap<TileIndex, TileIndex> appearance_cache;

  Data(TilesetInfo info, const UUID& id)
      : context {id},
        texture {std::move(info.texture)},
        tile_size {info.tile_size},
        row_count {texture->get_size().y / tile_size.y},
        column_count {texture->get_size().x / tile_size.x},
        uv_size {Float2 {tile_size} / Float2 {texture->get_size()}}
  {
  }
};

Tileset::Tileset(TilesetInfo info, const UUID& id)
    : mData {std::make_unique<Data>(std::move(info), id)}
{
  load_tiles();
}

Tileset::~Tileset() noexcept = default;

void Tileset::load_tiles()
{
  auto& data = *mData;
  const TileIndex count = tile_count();

  for (TileIndex index = 0; index < count; ++index) {
    const auto [row, col] = to_matrix_coords(index, data.column_count);

    auto tile = std::make_shared<Tile>(index);
    const auto tile_id = tile->get_uuid();

    const Int2 pos {col * data.tile_size.x, row * data.tile_size.y};
    tile->set_source({pos, data.tile_size});

    data.tiles.try_emplace(tile_id, std::move(tile));
    data.identifiers[index] = tile_id;
  }
}

void Tileset::accept(ContextVisitor& visitor) const
{
  visitor.visit(*this);
}

void Tileset::update()
{
  auto& data = *mData;
  data.appearance_cache.clear();

  // TODO usually a lot of tiles to visit here, store which tiles that need updating?
  //  for (const auto& tile_id : mAnimatedTiles) {
  //    auto& tile = lookup_in(mMetaTiles, tile_id);
  //    tile->update();
  //  }

  for (auto& [id, tile]: data.tiles) {
    tile->update();
  }
}

auto Tileset::operator[](const TileIndex index) -> Tile&
{
  return *get_tile_ptr(index);
}

auto Tileset::operator[](const TileIndex index) const -> const Tile&
{
  auto& data = *mData;
  const auto id = lookup_in(data.identifiers, index);
  return *lookup_in(data.tiles, id);
}

void Tileset::select_tile(const TileIndex index)
{
  if (index >= 0 && index < tile_count()) {
    mData->selected_tile = index;
  }
  else {
    throw Error {"Tried to select tileset tile with invalid index"};
  }
}

auto Tileset::get_selected_tile() const -> Maybe<TileIndex>
{
  return mData->selected_tile;
}

auto Tileset::get_tile_ptr(const TileIndex index) -> const Shared<Tile>&
{
  const auto& data = *mData;
  const auto id = lookup_in(data.identifiers, index);
  return lookup_in(data.tiles, id);
}

auto Tileset::index_of(const TilePos& pos) const -> TileIndex
{
  if (is_valid(pos)) {
    return pos.row() * column_count() + pos.col();
  }
  else {
    throw Error {"Invalid tile position!"};
  }
}

auto Tileset::appearance_of(const TileIndex index) const -> TileIndex
{
  const auto& data = *mData;

  if (const auto iter = data.appearance_cache.find(index);
      iter != data.appearance_cache.end()) {
    return iter->second;
  }

  if (const auto iter = data.identifiers.find(index); iter != data.identifiers.end()) {
    const auto& id = iter->second;
    const auto& tile = lookup_in(data.tiles, id);

    if (tile->is_animated()) {
      const auto appearance = tile->get_animation().current_frame().tile;
      data.appearance_cache[index] = appearance;
      return appearance;
    }
  }

  return index;
}

auto Tileset::get_ctx() -> ContextInfo&
{
  return mData->context;
}

auto Tileset::get_ctx() const -> const ContextInfo&
{
  return mData->context;
}

auto Tileset::get_uuid() const -> const UUID&
{
  return get_ctx().get_uuid();
}

auto Tileset::texture() const -> const Texture&
{
  return *mData->texture;
}

auto Tileset::tile_size() const noexcept -> const Int2&
{
  return mData->tile_size;
}

auto Tileset::uv_size() const noexcept -> const Float2&
{
  return mData->uv_size;
}

auto Tileset::row_count() const noexcept -> int32
{
  return mData->row_count;
}

auto Tileset::column_count() const noexcept -> int32
{
  return mData->column_count;
}

auto Tileset::tile_count() const noexcept -> int32
{
  return row_count() * column_count();
}

auto Tileset::is_valid(const TilePos& pos) const -> bool
{
  const auto row = pos.row();
  const auto col = pos.col();
  return row >= 0 && col >= 0 && row < row_count() && col < column_count();
}

auto Tileset::begin() const noexcept -> iterator
{
  return mData->tiles.begin();
}

auto Tileset::end() const noexcept -> iterator
{
  return mData->tiles.end();
}

}  // namespace tactile
