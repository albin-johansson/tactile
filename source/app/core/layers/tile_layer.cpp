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

#include "tile_layer.hpp"

#include <queue>  // queue

#include "core/layers/layer_visitor.hpp"
#include "core/tile_pos.hpp"
#include "core/utils/tiles.hpp"
#include "misc/assert.hpp"
#include "misc/panic.hpp"

namespace tactile::core {

TileLayer::TileLayer() : TileLayer{5, 5} {}

TileLayer::TileLayer(const usize rows, const usize columns)
    : mTiles{make_tile_matrix(rows, columns)}
{}

auto TileLayer::make() -> Shared<TileLayer>
{
  return std::make_shared<TileLayer>();
}

auto TileLayer::make(const usize rows, const usize columns) -> Shared<TileLayer>
{
  return std::make_shared<TileLayer>(rows, columns);
}

void TileLayer::accept(ILayerVisitor& visitor)
{
  visitor.visit(*this);
}

void TileLayer::accept(IConstLayerVisitor& visitor) const
{
  visitor.visit(*this);
}

void TileLayer::flood(const TilePos& origin,
                      const TileID replacement,
                      std::vector<TilePos>* affected)
{
  const auto target = tile_at(origin);

  if (!is_valid(origin) || (target == replacement)) {
    return;
  }

  std::queue<TilePos> positions;
  positions.push(origin);

  set_tile(origin, replacement);
  if (affected) {
    affected->push_back(origin);
  }

  /* Determines whether a position should be flooded */
  auto check = [&](const TilePos& position) {
    if (is_valid(position)) {
      const auto tile = tile_at(position);
      if (tile == target) {
        set_tile(position, replacement);
        if (affected) {
          affected->push_back(position);
        }
        positions.push(position);
      }
    }
  };

  while (!positions.empty()) {
    const auto position = positions.front();
    positions.pop();

    check(position.west());
    check(position.east());
    check(position.south());
    check(position.north());
  }
}

void TileLayer::add_row()
{
  mTiles.push_back(make_tile_row(column_count()));
}

void TileLayer::add_column() {}

void TileLayer::remove_row() {}

void TileLayer::remove_column() {}

void TileLayer::set_opacity(const float opacity)
{
  mDelegate.set_opacity(opacity);
}

void TileLayer::set_visible(const bool visible)
{
  mDelegate.set_visible(visible);
}

void TileLayer::set_tile(const TilePos& pos, const TileID id)
{
  if (is_valid(pos)) {
    mTiles[pos.row_index()][pos.col_index()] = id;
  }
  else {
    throw TactileError{"Invalid position!"};
  }
}

auto TileLayer::tile_at(const TilePos& pos) const -> TileID
{
  if (is_valid(pos)) {
    return mTiles[pos.row_index()][pos.col_index()];
  }
  else {
    throw TactileError{"Invalid position!"};
  }
}

auto TileLayer::is_valid(const TilePos& pos) const -> bool
{
  return pos.row() >= 0 &&                 //
         pos.col() >= 0 &&                 //
         pos.row_index() < row_count() &&  //
         pos.col_index() < column_count();
}

auto TileLayer::row_count() const -> usize
{
  return mTiles.size();
}

auto TileLayer::column_count() const -> usize
{
  TACTILE_ASSERT(mTiles.size() > 0);
  return mTiles.at(0).size();
}

auto TileLayer::get_opacity() const -> float
{
  return mDelegate.get_opacity();
}

auto TileLayer::is_visible() const -> bool
{
  return mDelegate.is_visible();
}

auto TileLayer::clone() const -> Shared<ILayer>
{
  auto layer = make();
  layer->mDelegate = mDelegate.clone();
  layer->mTiles = mTiles;
  return layer;
}

auto TileLayer::get_uuid() const -> const UUID&
{
  return mDelegate.get_uuid();
}

auto TileLayer::get_props() -> PropertyBundle&
{
  return mDelegate.get_props();
}

auto TileLayer::get_props() const -> const PropertyBundle&
{
  return mDelegate.get_props();
}

auto TileLayer::get_comps() -> ComponentBundle&
{
  return mDelegate.get_comps();
}

auto TileLayer::get_comps() const -> const ComponentBundle&
{
  return mDelegate.get_comps();
}

}  // namespace tactile::core
