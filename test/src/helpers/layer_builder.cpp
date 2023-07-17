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

#include "layer_builder.hpp"

#include <utility>  // move

#include "common/debug/assert.hpp"
#include "model/contexts/attached_component_system.hpp"
#include "model/contexts/context_components.hpp"
#include "model/entity_validation.hpp"
#include "model/layers/layer_components.hpp"
#include "model/layers/layer_factory.hpp"
#include "model/objects/object_components.hpp"
#include "model/objects/object_factory.hpp"

namespace tactile {

LayerBuilder::LayerBuilder(Registry& registry)
    : mRegistry {&registry}
{
}

auto LayerBuilder::with_id(const int32 id) -> Self&
{
  mId = id;
  return *this;
}

auto LayerBuilder::with_name(String name) -> Self&
{
  mName = std::move(name);
  return *this;
}

auto LayerBuilder::with_opacity(const float opacity) -> Self&
{
  mOpacity = opacity;
  return *this;
}

auto LayerBuilder::with_visibility(const bool visible) -> Self&
{
  mVisible = visible;
  return *this;
}

auto LayerBuilder::with_property(String name, Attribute value) -> Self&
{
  mProperties[std::move(name)] = std::move(value);
  return *this;
}

auto LayerBuilder::with_component(const Entity component_entity) -> Self&
{
  TACTILE_ASSERT(sys::is_component_entity(*mRegistry, component_entity));
  mComponents.push_back(component_entity);
  return *this;
}

auto LayerBuilder::as_tile_layer(const TileExtent extent) -> Self&
{
  mTiles = make_tile_matrix(extent);
  return *this;
}

auto LayerBuilder::set_tile(const TilePos pos, const TileID tile_id) -> Self&
{
  mTiles->at(static_cast<usize>(pos.row())).at(static_cast<usize>(pos.col())) = tile_id;
  return *this;
}

auto LayerBuilder::as_object_layer() -> Self&
{
  mObjects.emplace();
  return *this;
}

auto LayerBuilder::with_object(const int32 id,
                               const ObjectType type,
                               Entity* object_entity) -> Self&
{
  TACTILE_ASSERT(mObjects.has_value());

  const auto entity = sys::create_object(*mRegistry, type);
  mObjects->push_back(entity);

  auto& object = mRegistry->get<Object>(entity);
  object.meta_id = id;

  if (object_entity) {
    *object_entity = entity;
  }

  return *this;
}

auto LayerBuilder::as_group_layer() -> Self&
{
  mLayers.emplace();
  return *this;
}

auto LayerBuilder::with_layer(const Entity layer_entity) -> Self&
{
  TACTILE_ASSERT(mLayers.has_value());
  TACTILE_ASSERT(sys::is_layer_entity(*mRegistry, layer_entity));

  mLayers->push_back(layer_entity);
  return *this;
}

auto LayerBuilder::build() -> Entity
{
  auto& model = *mRegistry;
  Entity layer_entity = kNullEntity;

  if (mTiles.has_value()) {
    const TileExtent extent {mTiles->size(), mTiles->at(0).size()};
    layer_entity = sys::create_tile_layer(model, mId, extent);

    auto& tile_layer = model.get<TileLayer>(layer_entity);
    tile_layer.tiles = std::move(*mTiles);
  }

  if (mObjects.has_value()) {
    layer_entity = sys::create_object_layer(model, mId);

    auto& object_layer = model.get<ObjectLayer>(layer_entity);
    object_layer.objects = *mObjects;
  }

  if (mLayers.has_value()) {
    layer_entity = sys::create_group_layer(model, mId);

    auto& group_layer = model.get<GroupLayer>(layer_entity);
    group_layer.children = std::move(*mLayers);
  }

  TACTILE_ASSERT(layer_entity != kNullEntity);

  auto& layer = model.get<Layer>(layer_entity);
  layer.opacity = mOpacity;
  layer.visible = mVisible;

  auto& layer_context = model.get<Context>(layer_entity);
  layer_context.name = std::move(mName);
  layer_context.props = std::move(mProperties);

  for (const auto component_entity: mComponents) {
    sys::attach_component(model, layer_context, component_entity);
  }

  return layer_entity;
}

}  // namespace tactile
