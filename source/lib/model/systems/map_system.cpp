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

#include "map_system.hpp"

#include <utility>  // move

#include "common/debug/assert.hpp"
#include "common/debug/panic.hpp"
#include "common/util/algorithms.hpp"
#include "common/util/functional.hpp"
#include "common/util/vectors.hpp"
#include "model/components/context.hpp"
#include "model/components/document.hpp"
#include "model/components/tile_format.hpp"
#include "model/documents/document_system.hpp"
#include "model/layers/group_layer_ops.hpp"
#include "model/layers/layer_components.hpp"
#include "model/layers/layer_factory.hpp"
#include "model/layers/layer_recursion.hpp"
#include "model/layers/layer_tree_system.hpp"
#include "model/objects/object_system.hpp"
#include "model/systems/context/context_system.hpp"
#include "model/systems/tileset_system.hpp"
#include "model/systems/validation_system.hpp"
#include "model/tilesets/tileset_components.hpp"
#include "model/tilesets/tileset_factory.hpp"

namespace tactile::sys {
namespace {

[[nodiscard]] auto _create_layer(Model& model,
                                 Map& map,
                                 MapIdentifiers& identifiers,
                                 const LayerType type) -> Entity
{
  const auto layer_id = identifiers.next_layer_id++;
  switch (type) {
    case LayerType::TileLayer:
      return create_tile_layer(model, layer_id, map.extent);

    case LayerType::ObjectLayer:
      return create_object_layer(model, layer_id);

    case LayerType::GroupLayer:
      return create_group_layer(model, layer_id);

    default:
      throw TactileError {"Unexpected layer type"};
  }
}

[[nodiscard]] auto _determine_target_root_layer(Model& model, const Map& map)
    -> GroupLayer*
{
  GroupLayer* target_group = nullptr;

  if (map.active_layer != kNullEntity) {
    target_group = model.try_get<GroupLayer>(map.active_layer);
  }

  if (!target_group) {
    target_group = model.try_get<GroupLayer>(map.root_layer);
  }

  TACTILE_ASSERT(target_group != nullptr);
  return target_group;
}

}  // namespace

auto fix_tiles_in_map(Model& model, Map& map) -> FixTilesInMapResult
{
  FixTilesInMapResult result;

  visit_tile_layers(model, map, [&](const Entity layer_entity) {
    HashMap<TilePos, TileID> previous_tile_ids;

    auto& layer = model.get<TileLayer>(layer_entity);
    invoke_mn(map.extent.rows, map.extent.cols, [&](const usize row, const usize col) {
      TACTILE_ASSERT(row < layer.tiles.size());
      TACTILE_ASSERT(col < layer.tiles.at(row).size());
      const auto tile_id = layer.tiles[row][col];

      if (!is_valid_tile_identifier(model, map, tile_id)) {
        previous_tile_ids[TilePos::from(row, col)] = tile_id;
        layer.tiles[row][col] = kEmptyTile;
      }
    });

    result[layer_entity] = std::move(previous_tile_ids);
  });

  return result;
}

void restore_tiles_in_map(Model& model, const FixTilesInMapResult& invalid_tiles)
{
  for (const auto& [layer_entity, invalid_positions]: invalid_tiles) {
    auto& tile_layer = model.get<TileLayer>(layer_entity);

    for (const auto& [position, tile_id]: invalid_positions) {
      const auto row = static_cast<usize>(position.row());
      const auto col = static_cast<usize>(position.col());
      tile_layer.tiles[row][col] = tile_id;
    }
  }
}

void add_row_to_map(Model& model, Map& map)
{
  ++map.extent.rows;

  visit_tile_layers(model, map, [&](const Entity layer_entity) {
    auto& tile_layer = model.get<TileLayer>(layer_entity);

    TACTILE_ASSERT(tile_layer.tiles.at(0).size() == map.extent.cols);
    tile_layer.tiles.push_back(make_tile_row(map.extent.cols));
  });
}

void add_column_to_map(Model& model, Map& map)
{
  ++map.extent.cols;

  visit_tile_layers(model, map, [&](const Entity layer_entity) {
    auto& tile_layer = model.get<TileLayer>(layer_entity);

    for (auto& row: tile_layer.tiles) {
      row.push_back(kEmptyTile);
    }
  });
}

auto remove_row_from_map(Model& model, Map& map) -> Result
{
  if (map.extent.rows == 1) {
    return failure;
  }

  --map.extent.rows;

  visit_tile_layers(model, map, [&](const Entity layer_entity) {
    auto& tile_layer = model.get<TileLayer>(layer_entity);
    tile_layer.tiles.pop_back();
  });

  return success;
}

auto remove_column_from_map(Model& model, Map& map) -> Result
{
  if (map.extent.cols == 1) {
    return failure;
  }

  --map.extent.cols;

  visit_tile_layers(model, map, [&](const Entity layer_entity) {
    auto& tile_layer = model.get<TileLayer>(layer_entity);

    for (auto& row: tile_layer.tiles) {
      TACTILE_ASSERT(row.size() > 1);
      row.pop_back();
    }
  });

  return success;
}

auto resize_map(Model& model, Map& map, const TileExtent new_extent) -> Result
{
  if (new_extent.rows < 1 || new_extent.cols < 1) {
    return failure;
  }

  const auto old_extent = map.extent;
  map.extent = new_extent;

  const auto row_diff = udiff(old_extent.rows, new_extent.rows);
  const auto col_diff = udiff(old_extent.cols, new_extent.cols);

  if (old_extent.cols < new_extent.cols) {
    invoke_n(col_diff, [&] { add_column_to_map(model, map); });
  }
  else {
    invoke_n(col_diff, [&] { remove_column_from_map(model, map); });
  }

  if (old_extent.rows < new_extent.rows) {
    invoke_n(row_diff, [&] { add_row_to_map(model, map); });
  }
  else {
    invoke_n(row_diff, [&] { remove_row_from_map(model, map); });
  }

  return success;
}

auto add_new_layer_to_map(Model& model, const Entity map_entity, const LayerType type)
    -> Entity
{
  TACTILE_ASSERT(is_map_entity(model, map_entity));

  auto& map = model.get<Map>(map_entity);
  auto& map_identifiers = model.get<MapIdentifiers>(map_entity);

  const auto layer_entity = _create_layer(model, map, map_identifiers, type);

  auto* root_layer = _determine_target_root_layer(model, map);
  attach_layer_to(*root_layer, layer_entity);

  return layer_entity;
}

void attach_layer_to_map(Model& model,
                         Map& map,
                         const Entity layer_entity,
                         const Entity root_layer_entity)
{
  TACTILE_ASSERT(is_layer_entity(model, layer_entity));

  auto& root_layer = (root_layer_entity != kNullEntity)
                         ? model.get<GroupLayer>(root_layer_entity)
                         : model.get<GroupLayer>(map.root_layer);

  attach_layer_to(root_layer, layer_entity);
}

auto duplicate_layer(Model& model, const Entity map_entity, const Entity src_layer_entity)
    -> Entity
{
  TACTILE_ASSERT(is_map_entity(model, map_entity));
  TACTILE_ASSERT(is_layer_entity(model, src_layer_entity));

  const auto& map = model.get<Map>(map_entity);
  auto& map_identifiers = model.get<MapIdentifiers>(map_entity);

  const auto& src_layer = model.get<Layer>(src_layer_entity);
  const auto& src_context = model.get<Context>(src_layer_entity);

  const auto new_layer_entity = model.create_entity();

  auto& new_context = model.add<Context>(new_layer_entity);
  new_context = copy_context(model, src_context);

  auto& new_layer = model.add<Layer>(new_layer_entity);
  new_layer = src_layer;
  new_layer.id = map_identifiers.next_layer_id++;

  if (const auto* src_tile_layer = model.try_get<TileLayer>(src_layer_entity)) {
    auto& new_tile_layer = model.add<TileLayer>(new_layer_entity);
    new_tile_layer.tiles = src_tile_layer->tiles;
  }
  else if (const auto* src_object_layer = model.try_get<ObjectLayer>(src_layer_entity)) {
    auto& new_object_layer = model.add<ObjectLayer>(new_layer_entity);
    new_object_layer.objects.reserve(src_object_layer->objects.size());

    for (const auto src_object_entity: src_object_layer->objects) {
      new_object_layer.objects.push_back(duplicate_object(model, src_object_entity));
    }
  }
  else if (const auto* src_group_layer = model.try_get<GroupLayer>(src_layer_entity)) {
    auto& new_group_layer = model.add<GroupLayer>(new_layer_entity);
    new_group_layer.children.reserve(src_group_layer->children.size());

    for (const auto src_child_layer_entity: src_group_layer->children) {
      duplicate_layer(model, map_entity, src_child_layer_entity);
    }
  }

  const auto parent_layer_entity = get_parent_layer(model, map, src_layer_entity);

  if (parent_layer_entity != kNullEntity) {
    auto& parent_layer = model.get<GroupLayer>(parent_layer_entity);
    parent_layer.children.push_back(new_layer_entity);
  }
  else {
    auto& root_layer = model.get<GroupLayer>(map.root_layer);
    root_layer.children.push_back(new_layer_entity);
  }

  TACTILE_ASSERT(is_layer_entity(model, new_layer_entity));
  return new_layer_entity;
}

void remove_layer_from_map(Model& model, Map& map, const Entity layer_entity)
{
  if (map.active_layer == layer_entity) {
    map.active_layer = kNullEntity;
  }

  const auto parent_layer_entity = get_parent_layer(model, map, layer_entity);

  auto& parent_layer = model.get<GroupLayer>(parent_layer_entity);
  std::erase(parent_layer.children, layer_entity);
}

auto attach_tileset_to_map(Model& model,
                           const Entity map_entity,
                           const Entity tileset_entity) -> Entity
{
  TACTILE_ASSERT(is_map_entity(model, map_entity));
  TACTILE_ASSERT(is_tileset_entity(model, tileset_entity));

  auto& map = model.get<Map>(map_entity);
  auto& map_identifiers = model.get<MapIdentifiers>(map_entity);

  // TODO assert that the tileset isn't already attached
  const auto& tileset = model.get<Tileset>(tileset_entity);

  const auto attached_tileset_entity =
      create_attached_tileset(model, tileset_entity, map_identifiers.next_tile_id);

  map.attached_tilesets.push_back(attached_tileset_entity);
  map_identifiers.next_tile_id += (tileset.row_count * tileset.column_count) + 1;

  return attached_tileset_entity;
}

auto select_tileset(Model& model, Map& map, const Entity attached_tileset_entity)
    -> Result
{
  TACTILE_ASSERT(is_attached_tileset_entity(model, attached_tileset_entity));

  if (contained_in(map.attached_tilesets, attached_tileset_entity)) {
    map.active_tileset = attached_tileset_entity;
    return success;
  }

  return failure;
}

auto can_tile_row_be_removed(const Model& model) -> bool
{
  if (is_map_document_active(model)) {
    const auto document_entity = get_active_document(model);

    const auto& map_document = model.get<MapDocument>(document_entity);
    const auto& map = model.get<Map>(map_document.map);

    return map.extent.rows > 1;
  }

  return false;
}

auto can_tile_column_be_removed(const Model& model) -> bool
{
  if (is_map_document_active(model)) {
    const auto document_entity = get_active_document(model);

    const auto& map_document = model.get<MapDocument>(document_entity);
    const auto& map = model.get<Map>(map_document.map);

    return map.extent.cols > 1;
  }

  return false;
}

}  // namespace tactile::sys
