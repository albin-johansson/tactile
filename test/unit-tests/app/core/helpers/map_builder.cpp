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

#include "map_builder.hpp"

#include <utility>  // move

#include "core/common/functional.hpp"
#include "core/layers/tile_layer.hpp"
#include "core/tilesets/tileset.hpp"
#include "core/tilesets/tileset_info.hpp"

namespace tactile::test {

MapBuilder::MapBuilder() : mMap{std::make_shared<Map>()} {}

auto MapBuilder::build() -> MapBuilder
{
  return MapBuilder{};
}

auto MapBuilder::with_tile_size(const Vector2i& size) -> MapBuilder&
{
  mMap->set_tile_size(size);
  return *this;
}

auto MapBuilder::with_size(const usize rows, const usize columns) -> MapBuilder&
{
  mMap->resize(rows, columns);
  return *this;
}

auto MapBuilder::with_tile_layer(UUID* id, Maybe<TileID> initialValue) -> MapBuilder&
{
  const auto layerId = mMap->add_tile_layer();

  if (id) {
    *id = layerId;
  }

  if (initialValue) {
    auto& layer = mMap->view_tile_layer(layerId);
    invoke_mn(mMap->row_count(), mMap->column_count(), [&](usize r, usize c) {
      layer.set_tile(TilePos::from(r, c), *initialValue);
    });
  }

  return *this;
}

auto MapBuilder::with_tileset(UUID* id) -> MapBuilder&
{
  auto tileset = std::make_shared<Tileset>(TilesetInfo{
      .texture_path = "foo.png",
      .texture_id = 9,
      .texture_size = {1024, 1024},
      .tile_size = {32, 32},
  });

  if (id) {
    *id = tileset->get_uuid();
  }

  mMap->attach_tileset(std::move(tileset), false);

  return *this;
}

auto MapBuilder::result() -> const Shared<Map>&
{
  return mMap;
}

}  // namespace tactile::test
