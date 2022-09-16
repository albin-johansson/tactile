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

#include "core/comp/component_index.hpp"
#include "core/layer/object_layer.hpp"
#include "core/layer/tile_layer.hpp"
#include "core/tileset/tileset.hpp"
#include "core/tileset/tileset_info.hpp"
#include "core/util/functional.hpp"

namespace tactile::test {

MapBuilder::MapBuilder()
    : mDocument {std::make_unique<MapDocument>(float2 {32, 32}, 5, 5)}
{
  mDocument->set_component_index(std::make_shared<ComponentIndex>());
}

auto MapBuilder::build() -> MapBuilder { return MapBuilder {}; }

auto MapBuilder::with_tile_size(const int2& size) -> MapBuilder&
{
  mDocument->get_map().set_tile_size(size);
  return *this;
}

auto MapBuilder::with_size(const usize rows, const usize columns) -> MapBuilder&
{
  mDocument->get_map().resize(rows, columns);
  return *this;
}

auto MapBuilder::with_tile_layer(UUID* id, Maybe<TileID> initial_value) -> MapBuilder&
{
  auto& map = mDocument->get_map();

  const auto layer_id = map.add_tile_layer();
  mDocument->get_contexts().add_context(map.get_layer(layer_id));

  if (id) {
    *id = layer_id;
  }

  if (initial_value) {
    auto& layer = map.view_tile_layer(layer_id);
    invoke_mn(map.row_count(), map.column_count(), [&](const usize r, const usize c) {
      layer.set_tile(TilePos::from(r, c), *initial_value);
    });
  }

  return *this;
}

auto MapBuilder::with_object_layer(UUID* id) -> MapBuilder&
{
  auto& map = mDocument->get_map();

  const auto layer_id = map.add_object_layer();
  mDocument->get_contexts().add_context(map.get_layer(layer_id));

  if (id) {
    *id = layer_id;
  }

  return *this;
}

auto MapBuilder::with_object(const ObjectType type,
                             Shared<Object>* out_object,
                             UUID* out_layer) -> MapBuilder&
{
  auto& map = mDocument->get_map();

  if (!mDedicatedObjectLayer) {
    mDedicatedObjectLayer = map.add_object_layer();
    mDocument->get_contexts().add_context(map.get_layer(*mDedicatedObjectLayer));
  }

  auto object = std::make_shared<Object>();
  object->set_type(type);

  if (out_object) {
    *out_object = object;
  }

  if (out_layer) {
    *out_layer = *mDedicatedObjectLayer;
  }

  mDocument->get_contexts().add_context(object);

  auto& layer = map.view_object_layer(*mDedicatedObjectLayer);
  layer.add_object(std::move(object));

  return *this;
}

auto MapBuilder::with_tileset(UUID* id) -> MapBuilder&
{
  auto tileset = std::make_shared<Tileset>(TilesetInfo {
      .texture_path = "foo.png",
      .texture_id = 9,
      .texture_size = {1024, 1024},
      .tile_size = {32, 32},
  });

  if (id) {
    *id = tileset->get_uuid();
  }

  mDocument->get_contexts().add_context(tileset);

  auto& map = mDocument->get_map();
  map.attach_tileset(std::move(tileset), false);

  return *this;
}

auto MapBuilder::result() -> Unique<MapDocument> { return std::move(mDocument); }

}  // namespace tactile::test
