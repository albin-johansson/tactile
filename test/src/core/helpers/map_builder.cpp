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
#include "core/layer/group_layer.hpp"
#include "core/layer/object_layer.hpp"
#include "core/layer/tile_layer.hpp"
#include "core/tile/tileset.hpp"
#include "core/tile/tileset_bundle.hpp"
#include "core/tile/tileset_info.hpp"
#include "core/util/functional.hpp"
#include "io/load_texture.hpp"

namespace tactile::test {

MapBuilder::MapBuilder()
    : mDocument {std::make_unique<MapDocument>(Float2 {32, 32}, 5, 5)}
{
  mDocument->set_component_index(std::make_shared<ComponentIndex>());
}

auto MapBuilder::build() -> MapBuilder
{
  return MapBuilder {};
}

auto MapBuilder::with_tile_size(const Int2& size) -> MapBuilder&
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
  auto& root = map.invisible_root();

  const auto layer_id = map.add_tile_layer();
  mDocument->get_contexts().add_context(root.ptr(layer_id));

  if (id) {
    *id = layer_id;
  }

  if (initial_value) {
    auto& layer = root.tile_layer(layer_id);
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
  mDocument->get_contexts().add_context(map.invisible_root().ptr(layer_id));

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
  auto& root = map.invisible_root();

  if (!mDedicatedObjectLayer) {
    mDedicatedObjectLayer = map.add_object_layer();
    mDocument->get_contexts().add_context(root.ptr(*mDedicatedObjectLayer));
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

  auto& layer = root.object_layer(*mDedicatedObjectLayer);
  layer.add_object(std::move(object));

  return *this;
}

auto MapBuilder::with_tileset(UUID* id) -> MapBuilder&
{
  auto tileset = std::make_shared<Tileset>(TilesetInfo {
      .texture = io::load_texture("resources/terrain.png"),
      .tile_size = {32, 32},
  });

  if (id) {
    *id = tileset->uuid();
  }

  mDocument->get_contexts().add_context(tileset);

  auto& map = mDocument->get_map();
  map.tileset_bundle().attach_tileset(std::move(tileset), false);

  return *this;
}

auto MapBuilder::result() -> Unique<MapDocument>
{
  return std::move(mDocument);
}

}  // namespace tactile::test
