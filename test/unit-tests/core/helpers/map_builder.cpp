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
#include "core/layers/object_layer.hpp"
#include "core/layers/tile_layer.hpp"
#include "core/tilesets/tileset.hpp"
#include "core/tilesets/tileset_info.hpp"

namespace tactile::test {

MapBuilder::MapBuilder()
    : mDocument{std::make_unique<MapDocument>(Vector2f{32, 32}, 5, 5)}
{}

auto MapBuilder::build() -> MapBuilder
{
  return MapBuilder{};
}

auto MapBuilder::with_tile_size(const Vector2i& size) -> MapBuilder&
{
  mDocument->get_map().set_tile_size(size);
  return *this;
}

auto MapBuilder::with_size(const usize rows, const usize columns) -> MapBuilder&
{
  mDocument->get_map().resize(rows, columns);
  return *this;
}

auto MapBuilder::with_tile_layer(UUID* id, Maybe<TileID> initialValue) -> MapBuilder&
{
  auto&      map = mDocument->get_map();
  const auto layerId = map.add_tile_layer();

  if (id) {
    *id = layerId;
  }

  if (initialValue) {
    auto& layer = map.view_tile_layer(layerId);
    invoke_mn(map.row_count(), map.column_count(), [&](usize r, usize c) {
      layer.set_tile(TilePos::from(r, c), *initialValue);
    });
  }

  return *this;
}

auto MapBuilder::with_object_layer(UUID* id) -> MapBuilder&
{
  const auto layerId = mDocument->get_map().add_object_layer();

  if (id) {
    *id = layerId;
  }

  return *this;
}

auto MapBuilder::with_object(const ObjectType type, Shared<Object>* outObject)
    -> MapBuilder&
{
  auto& map = mDocument->get_map();

  if (!mDedicatedObjectLayer) {
    mDedicatedObjectLayer = map.add_object_layer();
  }

  auto object = std::make_shared<Object>();
  object->set_type(type);

  if (outObject) {
    *outObject = object;
  }

  auto& layer = map.view_object_layer(*mDedicatedObjectLayer);
  layer.add_object(std::move(object));

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

  mDocument->get_map().attach_tileset(std::move(tileset), false);

  return *this;
}

auto MapBuilder::result() -> Unique<MapDocument>
{
  return std::move(mDocument);
}

}  // namespace tactile::test
