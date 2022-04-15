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

#include "object_system.hpp"

#include <optional>  // optional
#include <string>    // string
#include <utility>   // move

#include "context_system.hpp"
#include "core/components/layers.hpp"
#include "core/components/objects.hpp"
#include "core/map.hpp"
#include "core/systems/layers/layer_system.hpp"
#include "core/systems/layers/object_layer_system.hpp"
#include "misc/throw.hpp"
#include "registry_system.hpp"

namespace tactile::sys {
namespace {

[[nodiscard]] auto _make_object(entt::registry& registry,
                                const LayerID layerId,
                                std::string name,
                                const ObjectType type,
                                const float x,
                                const float y,
                                const float width,
                                const float height) -> ObjectID
{
  auto& map = registry.ctx<MapInfo>();
  const auto id = map.next_object_id;
  ++map.next_object_id;

  auto&& [layerEntity, objectLayer] = get_object_layer(registry, layerId);

  const auto objectEntity = registry.create();
  objectLayer.objects.push_back(objectEntity);

  auto& context = add_attribute_context(registry, objectEntity);
  context.name = std::move(name);

  auto& object = registry.emplace<comp::Object>(objectEntity);
  object.id = id;
  object.type = type;

  object.x = x;
  object.y = y;
  object.width = width;
  object.height = height;
  object.visible = true;

  return id;
}

}  // namespace

auto make_rectangle_object(entt::registry& registry,
                           const LayerID layerId,
                           const float x,
                           const float y,
                           const float width,
                           const float height) -> ObjectID
{
  return _make_object(registry,
                      layerId,
                      "Rectangle",
                      ObjectType::rect,
                      x,
                      y,
                      width,
                      height);
}

auto make_ellipse_object(entt::registry& registry,
                         const LayerID layerId,
                         const float x,
                         const float y,
                         const float width,
                         const float height) -> ObjectID
{
  return _make_object(registry,
                      layerId,
                      "Ellipse",
                      ObjectType::ellipse,
                      x,
                      y,
                      width,
                      height);
}

auto make_point_object(entt::registry& registry,
                       const LayerID layerId,
                       const float x,
                       const float y) -> ObjectID
{
  return _make_object(registry, layerId, "Point", ObjectType::point, x, y, 0, 0);
}

auto remove_object(entt::registry& registry, const ObjectID id) -> RemoveObjectResult
{
  const auto objectEntity = find_object(registry, id);

  std::optional<LayerID> layerId;
  for (auto&& [layerEntity, layer, objectLayer] :
       registry.view<comp::Layer, comp::ObjectLayer>().each()) {
    if (std::erase(objectLayer.objects, objectEntity) > 0) {
      layerId = layer.id;
      break;
    }
  }

  if (auto& activeContext = registry.ctx<comp::ActiveAttributeContext>();
      activeContext.entity == objectEntity) {
    activeContext.entity = entt::null;
  }

  if (auto& activeObject = registry.ctx<comp::ActiveObject>();
      activeObject.entity == objectEntity) {
    activeObject.entity = entt::null;
  }

  RemoveObjectResult result;
  result.layer = layerId.value();
  result.object = checked_get<comp::Object>(registry, objectEntity);
  result.context = copy_attribute_context(registry, objectEntity);

  registry.destroy(objectEntity);

  return result;
}

void restore_object(entt::registry& registry, RemoveObjectResult snapshot)
{
  const auto objectEntity = registry.create();

  registry.emplace<comp::Object>(objectEntity, std::move(snapshot.object));
  restore_attribute_context(registry, objectEntity, std::move(snapshot.context));

  auto&& [layerEntity, layer] = get_object_layer(registry, snapshot.layer);
  layer.objects.push_back(objectEntity);
}

auto find_object(const entt::registry& registry, const ObjectID id) -> entt::entity
{
  for (auto&& [entity, object] : registry.view<comp::Object>().each()) {
    if (object.id == id) {
      return entity;
    }
  }

  return entt::null;
}

auto get_object(const entt::registry& registry, const ObjectID id) -> entt::entity
{
  const auto entity = find_object(registry, id);
  if (entity != entt::null) {
    return entity;
  }
  else {
    throw_traced(TactileError{"Invalid object identifier!"});
  }
}

}  // namespace tactile::sys
