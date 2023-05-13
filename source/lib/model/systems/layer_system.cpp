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
#include "common/type/string.hpp"
#include "core/context.hpp"
#include "core/layer.hpp"
#include "core/tile_matrix.hpp"
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

auto duplicate_layer(Model& model,
                     const Entity root_layer_entity,
                     const Entity source_layer_entity,
                     const int32 new_id) -> Entity
{
  TACTILE_ASSERT(root_layer_entity == kNullEntity ||
                 is_group_layer_entity(model, root_layer_entity));
  TACTILE_ASSERT(is_layer_entity(model, source_layer_entity));

  const auto parent_layer_entity =
      get_parent_layer(model, root_layer_entity, source_layer_entity);

  const auto& src_layer = model.get<Layer>(source_layer_entity);
  const auto& src_context = model.get<Context>(source_layer_entity);

  const auto new_layer_entity = model.create_entity();

  auto& new_context = model.add<Context>(new_layer_entity);
  new_context = copy_context(model, src_context);

  auto& new_layer = model.add<Layer>(new_layer_entity);
  new_layer = src_layer;
  new_layer.id = new_id;

  if (const auto* src_tile_layer = model.try_get<TileLayer>(source_layer_entity)) {
    auto& new_tile_layer = model.add<TileLayer>(new_layer_entity);
    new_tile_layer.tiles = src_tile_layer->tiles;
  }

  if (const auto* src_object_layer = model.try_get<ObjectLayer>(source_layer_entity)) {
    auto& new_object_layer = model.add<ObjectLayer>(new_layer_entity);
    new_object_layer.objects.reserve(src_object_layer->objects.size());

    for (const auto src_object_entity: src_object_layer->objects) {
      new_object_layer.objects.push_back(duplicate_object(model, src_object_entity));
    }
  }

  if (const auto* src_group_layer = model.try_get<GroupLayer>(source_layer_entity)) {
    auto& new_group_layer = model.add<GroupLayer>(new_layer_entity);
    new_group_layer.children.reserve(src_group_layer->children.size());

    for (const auto src_child_layer_entity: src_group_layer->children) {
      new_group_layer.children.push_back(duplicate_layer(model,
                                                         source_layer_entity,
                                                         src_child_layer_entity,
                                                         new_id + 1));
    }
  }

  // TODO copy tile, object, or group layer

  TACTILE_ASSERT(is_layer_entity(model, new_layer_entity));
  return new_layer_entity;
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
