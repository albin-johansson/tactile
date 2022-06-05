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

#include <string>   // string
#include <utility>  // move

#include <entt/entity/registry.hpp>

#include "core/common/ecs.hpp"
#include "core/common/maybe.hpp"
#include "core/components/layers.hpp"
#include "core/components/map_info.hpp"
#include "core/components/objects.hpp"
#include "core/systems/context_system.hpp"
#include "core/systems/layers/layer_system.hpp"
#include "core/systems/layers/object_layer_system.hpp"
#include "misc/panic.hpp"

namespace tactile::sys {
namespace {

[[nodiscard]] auto _make_object(entt::registry& registry,
                                const UUID& layerId,
                                std::string name,
                                const ObjectType type,
                                const float x,
                                const float y,
                                const float width,
                                const float height) -> ObjectID
{
  auto& map = ctx_get<comp::MapInfo>(registry);
  const auto id = map.next_object_id;
  ++map.next_object_id;

  auto&& [layerEntity, objectLayer] = get_object_layer(registry, layerId);

  const auto objectEntity = registry.create();
  objectLayer.objects.push_back(objectEntity);

  auto& context = register_context(registry, objectEntity);
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

auto new_rectangle_object(entt::registry& registry,
                          const UUID& layerId,
                          const float x,
                          const float y,
                          const float width,
                          const float height) -> ObjectID
{
  return _make_object(registry,
                      layerId,
                      "Rectangle",
                      ObjectType::Rect,
                      x,
                      y,
                      width,
                      height);
}

auto new_ellipse_object(entt::registry& registry,
                        const UUID& layerId,
                        const float x,
                        const float y,
                        const float width,
                        const float height) -> ObjectID
{
  return _make_object(registry,
                      layerId,
                      "Ellipse",
                      ObjectType::Ellipse,
                      x,
                      y,
                      width,
                      height);
}

auto new_point_object(entt::registry& registry,
                      const UUID& layerId,
                      const float x,
                      const float y) -> ObjectID
{
  return _make_object(registry, layerId, "Point", ObjectType::Point, x, y, 0, 0);
}

auto remove_object(entt::registry& registry, const ObjectID id) -> RemoveObjectResult
{
  const auto objectEntity = get_object(registry, id);

  Maybe<UUID> layerId;
  for (auto&& [layerEntity, context, layer, objectLayer] :
       registry.view<comp::Context, comp::Layer, comp::ObjectLayer>().each()) {
    if (std::erase(objectLayer.objects, objectEntity) > 0) {
      layerId = context.id;
      break;
    }
  }

  auto& active = ctx_get<comp::ActiveState>(registry);

  if (active.context == objectEntity) {
    active.context = entt::null;
  }

  if (active.object == objectEntity) {
    active.object = entt::null;
  }

  RemoveObjectResult result;
  result.layer = layerId.value();
  result.object = checked_get<comp::Object>(registry, objectEntity);
  result.context = copy_attribute_context(registry, objectEntity);

  destroy_entity(registry, objectEntity);

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
  return find_one<comp::Object>(registry, [id](const comp::Object& object) {
    return object.id == id;
  });
}

auto get_object(const entt::registry& registry, const ObjectID id) -> entt::entity
{
  const auto entity = find_object(registry, id);
  if (entity != entt::null) {
    return entity;
  }
  else {
    throw TactileError{"Invalid object identifier!"};
  }
}

}  // namespace tactile::sys
