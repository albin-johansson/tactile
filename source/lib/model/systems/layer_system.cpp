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

#include "layer_system.hpp"

#include <utility>  // move

#include "common/debug/assert.hpp"
#include "common/tile_matrix.hpp"
#include "common/type/string.hpp"
#include "components/context.hpp"
#include "components/layer.hpp"
#include "model/systems/context/context_system.hpp"
#include "model/systems/object_system.hpp"
#include "model/systems/validation.hpp"

namespace tactile::sys {
namespace {

[[nodiscard]] auto _create_layer_entity_base(Model& model,
                                             const LayerType type,
                                             String name,
                                             const int32 id) -> Entity
{
  const auto layer_entity = model.create_entity();

  auto& context = model.add<Context>(layer_entity);
  context.name = std::move(name);

  auto& layer = model.add<Layer>(layer_entity);
  layer.type = type;
  layer.id = id;
  layer.opacity = 1.0f;
  layer.visible = true;

  return layer_entity;
}

}  // namespace

auto create_tile_layer(Model& model, const int32 id, const TileExtent extent) -> Entity
{
  const auto layer_entity =
      _create_layer_entity_base(model, LayerType::TileLayer, "Tile Layer", id);

  auto& tile_layer = model.add<TileLayer>(layer_entity);
  tile_layer.tiles = make_tile_matrix(extent);

  TACTILE_ASSERT(is_tile_layer_entity(model, layer_entity));
  return layer_entity;
}

auto create_object_layer(Model& model, const int32 id) -> Entity
{
  const auto layer_entity =
      _create_layer_entity_base(model, LayerType::ObjectLayer, "Object Layer", id);

  model.add<ObjectLayer>(layer_entity);

  TACTILE_ASSERT(is_object_layer_entity(model, layer_entity));
  return layer_entity;
}

auto create_group_layer(Model& model, const int32 id) -> Entity
{
  const auto layer_entity =
      _create_layer_entity_base(model, LayerType::GroupLayer, "Group Layer", id);

  model.add<GroupLayer>(layer_entity);

  TACTILE_ASSERT(is_group_layer_entity(model, layer_entity));
  return layer_entity;
}

void recurse_layers(const Model& model,
                    const Entity root_layer_entity,
                    const EntityCallback& callback)
{
  TACTILE_ASSERT(is_group_layer_entity(model, root_layer_entity));
  const auto& root_layer = model.get<GroupLayer>(root_layer_entity);

  for (const auto layer_entity: root_layer.children) {
    if (model.try_get<GroupLayer>(layer_entity) != nullptr) {
      recurse_layers(model, layer_entity, callback);
    }

    callback(layer_entity);
  }
}

void recurse_tile_layers(Model& model,
                         const Entity root_layer_entity,
                         const TileLayerCallback& callback)
{
  recurse_layers(model, root_layer_entity, [&](const Entity layer_entity) {
    if (auto* tile_layer = model.try_get<TileLayer>(layer_entity)) {
      callback(layer_entity, *tile_layer);
    }
  });
}

void recurse_tile_layers(const Model& model,
                         const Entity root_layer_entity,
                         const ConstTileLayerCallback& callback)
{
  recurse_layers(model, root_layer_entity, [&](const Entity layer_entity) {
    if (const auto* tile_layer = model.try_get<TileLayer>(layer_entity)) {
      callback(layer_entity, *tile_layer);
    }
  });
}

auto get_parent_layer(const Model& model,
                      const Entity root_layer_entity,
                      const Entity target_layer_entity) -> Entity
{
  TACTILE_ASSERT(is_group_layer_entity(model, root_layer_entity));
  TACTILE_ASSERT(is_layer_entity(model, target_layer_entity));

  const auto& root_layer = model.get<GroupLayer>(root_layer_entity);

  for (const auto layer_entity: root_layer.children) {
    if (layer_entity == target_layer_entity) {
      return root_layer_entity;
    }
    else if (model.has<GroupLayer>(layer_entity)) {
      const auto parent_entity =
          get_parent_layer(model, layer_entity, target_layer_entity);

      if (parent_entity != kNullEntity) {
        return parent_entity;
      }
    }
  }

  return kNullEntity;
}

}  // namespace tactile::sys
