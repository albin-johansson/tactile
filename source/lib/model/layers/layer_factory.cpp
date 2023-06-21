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

#include "layer_factory.hpp"

#include <utility>  // move

#include "common/debug/assert.hpp"
#include "common/type/string.hpp"
#include "model/components/context.hpp"
#include "model/layers/layer_components.hpp"
#include "model/systems/validation_system.hpp"

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

}  // namespace tactile::sys
