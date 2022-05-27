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

#include "object_layer_system.hpp"

#include <entt/entity/registry.hpp>

#include "core/common/ecs.hpp"
#include "core/components/map_info.hpp"
#include "core/components/objects.hpp"
#include "core/systems/context_system.hpp"
#include "core/systems/layers/layer_system.hpp"
#include "core/systems/viewport_system.hpp"
#include "misc/panic.hpp"

namespace tactile::sys {
namespace {

[[nodiscard]] auto _get_hit_detection_bounds(const comp::Object& object,
                                             const Vector2f& mapTileSize,
                                             const float xRatio,
                                             const float yRatio) -> cen::frect
{
  /* Points have no width or height, so we have to create a hitbox large enough to be easy
     for the user to click */
  if (object.type == ObjectType::Point) {
    const auto width = static_cast<float>(mapTileSize.x) / 2.0f;
    const auto height = static_cast<float>(mapTileSize.y) / 2.0f;

    return {(object.x - (width / 2.0f)) * xRatio,
            (object.y - (height / 2.0f)) * yRatio,
            width * xRatio,
            height * yRatio};
  }
  else {
    return {object.x * xRatio,
            object.y * yRatio,
            object.width * xRatio,
            object.height * yRatio};
  }
}

}  // namespace

auto get_object_layer(entt::registry& registry, const LayerID id)
    -> std::pair<entt::entity, comp::ObjectLayer&>
{
  const auto entity = find_layer(registry, id);
  if (entity != entt::null && registry.all_of<comp::ObjectLayer>(entity)) {
    return {entity, registry.get<comp::ObjectLayer>(entity)};
  }
  else {
    throw TactileError{"Invalid object layer ID!"};
  }
}

auto get_object_layer(const entt::registry& registry, const LayerID id)
    -> std::pair<entt::entity, const comp::ObjectLayer&>
{
  const auto entity = find_layer(registry, id);
  if (entity != entt::null && registry.all_of<comp::ObjectLayer>(entity)) {
    return {entity, registry.get<comp::ObjectLayer>(entity)};
  }
  else {
    throw TactileError{"Invalid object layer ID!"};
  }
}

auto has_object(const entt::registry& registry,
                const comp::ObjectLayer& layer,
                const ObjectID id) -> bool
{
  return find_object(registry, layer, id) != entt::null;
}

auto find_object(const entt::registry& registry,
                 const comp::ObjectLayer& layer,
                 const ObjectID id) -> entt::entity
{
  for (const auto objectEntity : layer.objects) {
    const auto& object = checked_get<comp::Object>(registry, objectEntity);
    if (object.id == id) {
      return objectEntity;
    }
  }

  return entt::null;
}

auto find_object(const entt::registry& registry,
                 const comp::ObjectLayer& layer,
                 const float x,
                 const float y) -> entt::entity
{
  const auto& map = ctx_get<comp::MapInfo>(registry);
  const auto [xRatio, yRatio] = get_viewport_scaling_ratio(registry);

  for (const auto objectEntity : layer.objects) {
    const auto& object = checked_get<comp::Object>(registry, objectEntity);
    const auto bounds = _get_hit_detection_bounds(object, map.tile_size, xRatio, yRatio);
    if (bounds.contains({x, y})) {
      return objectEntity;
    }
  }

  return entt::null;
}

}  // namespace tactile::sys
