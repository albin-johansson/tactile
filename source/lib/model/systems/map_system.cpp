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
#include "common/util/algorithm.hpp"
#include "common/util/functional.hpp"
#include "components/context.hpp"
#include "components/document.hpp"
#include "core/layer.hpp"
#include "core/tile_format.hpp"
#include "core/tileset.hpp"
#include "model/systems/document_system.hpp"
#include "model/systems/layer_system.hpp"
#include "model/systems/tileset_system.hpp"
#include "model/systems/validation.hpp"

namespace tactile::sys {
namespace {

[[nodiscard]] auto _create_layer(Model& model, Map& map, const LayerType type) -> Entity
{
  const auto layer_id = map.next_layer_id++;
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

auto create_map(Model& model, const TileExtent& extent, const Int2& tile_size) -> Entity
{
  const auto map_entity = model.create_entity();

  auto& map_context = model.add<Context>(map_entity);
  map_context.name = "Map";

  auto& map = model.add<Map>(map_entity);
  map.extent = extent;
  map.tile_size = tile_size;
  map.root_layer = create_group_layer(model, -1);

  model.add<TileFormat>(map_entity);

  return map_entity;
}

auto fix_tiles_in_map(Model& model, Map& map) -> FixTilesInMapResult
{
  FixTilesInMapResult result;

  recurse_tile_layers(model, map.root_layer, [&](Entity layer_entity, TileLayer& layer) {
    HashMap<TilePos, TileID> previous_tile_ids;

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

  recurse_tile_layers(model, map.root_layer, [&](Entity, TileLayer& tile_layer) {
    TACTILE_ASSERT(tile_layer.tiles.at(0).size() == map.extent.cols);
    tile_layer.tiles.push_back(make_tile_row(map.extent.cols));
  });
}

void add_column_to_map(Model& model, Map& map)
{
  ++map.extent.cols;

  recurse_tile_layers(model, map.root_layer, [&](Entity, TileLayer& tile_layer) {
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

  recurse_tile_layers(model, map.root_layer, [&](Entity, TileLayer& tile_layer) {
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

  recurse_tile_layers(model, map.root_layer, [&](Entity, TileLayer& tile_layer) {
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
    invoke_n(col_diff, [&] { sys::add_column_to_map(model, map); });
  }
  else {
    invoke_n(col_diff, [&] { sys::remove_column_from_map(model, map); });
  }

  if (old_extent.rows < new_extent.rows) {
    invoke_n(row_diff, [&] { sys::add_row_to_map(model, map); });
  }
  else {
    invoke_n(row_diff, [&] { sys::remove_row_from_map(model, map); });
  }

  return success;
}

auto add_new_layer_to_map(Model& model, Map& map, const LayerType type) -> Entity
{
  const auto layer_entity = _create_layer(model, map, type);

  auto* root_layer = _determine_target_root_layer(model, map);
  root_layer->append(layer_entity);

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

  root_layer.append(layer_entity);
}

void remove_layer_from_map(Model& model, Map& map, const Entity layer_entity)
{
  auto& root_layer = model.get<GroupLayer>(map.root_layer);

  if (map.active_layer == layer_entity) {
    map.active_layer = kNullEntity;
  }

  recurse_layers(model, map.root_layer, [&](const Entity child_layer_entity) {
    if (auto* child_layer_group = model.try_get<GroupLayer>(child_layer_entity)) {
      std::erase(child_layer_group->children, layer_entity);
    }
  });
}

auto attach_tileset_to_map(Model& model, Map& map, const Entity tileset_entity)
    -> AttachedTilesetEntity
{
  // TODO assert that the tileset isn't already attached
  TACTILE_ASSERT(is_tileset_entity(model, tileset_entity));
  const auto& tileset = model.get<Tileset>(tileset_entity);

  const auto attached_tileset_entity =
      create_attached_tileset(model, tileset_entity, map.next_tile_id);

  map.attached_tilesets.push_back(attached_tileset_entity);
  map.next_tile_id += (tileset.row_count * tileset.column_count) + 1;

  return attached_tileset_entity;
}

auto can_tile_row_be_removed(const Model& model) -> bool
{
  if (sys::is_map_document_active(model)) {
    const auto document_entity = sys::get_active_document(model);

    const auto& map_document = model.get<MapDocument>(document_entity);
    const auto& map = model.get<Map>(map_document.map);

    return map.extent.rows > 1;
  }

  return false;
}

auto can_tile_column_be_removed(const Model& model) -> bool
{
  if (sys::is_map_document_active(model)) {
    const auto document_entity = sys::get_active_document(model);

    const auto& map_document = model.get<MapDocument>(document_entity);
    const auto& map = model.get<Map>(map_document.map);

    return map.extent.cols > 1;
  }

  return false;
}

}  // namespace tactile::sys
