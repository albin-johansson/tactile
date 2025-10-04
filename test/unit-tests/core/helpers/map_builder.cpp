// Copyright (C) 2025 Albin Johansson
// This software is provided under the terms of the MIT License.

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
    : mDocument {std::make_unique<MapDocument>(Vector2f {32, 32}, 5, 5)}
{
  mDocument->set_component_index(std::make_shared<ComponentIndex>());
}

auto MapBuilder::build() -> MapBuilder
{
  return MapBuilder {};
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
  auto& map = mDocument->get_map();

  const auto layerId = map.add_tile_layer();
  mDocument->get_contexts().add_context(map.get_layer(layerId));

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
  auto& map = mDocument->get_map();

  const auto layerId = map.add_object_layer();
  mDocument->get_contexts().add_context(map.get_layer(layerId));

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
    mDocument->get_contexts().add_context(map.get_layer(*mDedicatedObjectLayer));
  }

  auto object = std::make_shared<Object>();
  object->set_type(type);

  if (outObject) {
    *outObject = object;
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

auto MapBuilder::result() -> Unique<MapDocument>
{
  return std::move(mDocument);
}

}  // namespace tactile::test
