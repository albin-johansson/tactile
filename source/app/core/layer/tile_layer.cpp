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

#include <queue>    // queue
#include <utility>  // move

#include "core/common/functional.hpp"
#include "core/common/math.hpp"
#include "core/ctx/context_visitor.hpp"
#include "core/layer/layer_visitor.hpp"
#include "core/tile_pos.hpp"
#include "core/util/tiles.hpp"
#include "misc/assert.hpp"
#include "misc/panic.hpp"

namespace tactile {

TileLayer::TileLayer() : TileLayer {5, 5} {}

TileLayer::TileLayer(const usize rows, const usize columns)
    : mTiles {make_tile_matrix(rows, columns)}
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

void TileLayer::flood(const TilePos&        origin,
                      const TileID          replacement,
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

void TileLayer::add_column()
{
  for (auto& row : mTiles) {
    row.push_back(empty_tile);
  }
}

void TileLayer::remove_row()
{
  TACTILE_ASSERT(row_count() > 1);
  mTiles.pop_back();
}

void TileLayer::remove_column()
{
  for (auto& row : mTiles) {
    TACTILE_ASSERT(row.size() > 1);
    row.pop_back();
  }
}

void TileLayer::resize(const usize rows, const usize columns)
{
  const auto currentRows = row_count();
  const auto currentCols = column_count();

  if (const auto n = udiff(currentRows, rows); currentRows < rows) {
    invoke_n(n, [this] { add_row(); });
  }
  else {
    invoke_n(n, [this] { remove_row(); });
  }

  if (const auto n = udiff(currentCols, columns); currentCols < columns) {
    invoke_n(n, [this] { add_column(); });
  }
  else {
    invoke_n(n, [this] { remove_column(); });
  }
}

void TileLayer::set_opacity(const float opacity)
{
  mDelegate.set_opacity(opacity);
}

void TileLayer::set_visible(const bool visible)
{
  mDelegate.set_visible(visible);
}

void TileLayer::set_parent(const Maybe<UUID>& parentId)
{
  mDelegate.set_parent(parentId);
}

void TileLayer::set_meta_id(const int32 id)
{
  mDelegate.set_meta_id(id);
}

void TileLayer::accept(IContextVisitor& visitor) const
{
  visitor.visit(*this);
}

void TileLayer::set_name(std::string name)
{
  mDelegate.set_name(std::move(name));
}

void TileLayer::set_tile(const TilePos& pos, const TileID id)
{
  if (is_valid(pos)) [[likely]] {
    mTiles[pos.urow()][pos.ucol()] = id;
  }
  else {
    throw TactileError {"Invalid position!"};
  }
}

void TileLayer::set_tiles(const TileCache& cache)
{
  for (const auto& [pos, tile] : cache) {
    set_tile(pos, tile);
  }
}

auto TileLayer::tile_at(const TilePos& pos) const -> TileID
{
  if (is_valid(pos)) [[likely]] {
    return mTiles[pos.urow()][pos.ucol()];
  }
  else {
    throw TactileError {"Invalid position!"};
  }
}

auto TileLayer::is_valid(const TilePos& pos) const -> bool
{
  return pos.row() >= 0 &&            //
         pos.col() >= 0 &&            //
         pos.urow() < row_count() &&  //
         pos.ucol() < column_count();
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

auto TileLayer::get_tiles() const -> const TileMatrix&
{
  return mTiles;
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

auto TileLayer::get_name() const -> const std::string&
{
  return mDelegate.get_name();
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

auto TileLayer::get_parent() const -> Maybe<UUID>
{
  return mDelegate.get_parent();
}

auto TileLayer::get_meta_id() const -> Maybe<int32>
{
  return mDelegate.get_meta_id();
}

}  // namespace tactile
