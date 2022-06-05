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
#include "core/components/layers.hpp"
#include "core/components/map_info.hpp"
#include "core/components/objects.hpp"
#include "core/systems/context_system.hpp"
#include "core/systems/viewport_system.hpp"

namespace tactile::sys {
namespace {

[[nodiscard]] auto _get_hit_detection_bounds(const comp::Object& object,
                                             const Vector2f& mapTileSize,
                                             const Vector2f& ratio) -> cen::frect
{
  /* Points have no width or height, so we have to create a hitbox large enough to be easy
     for the user to click */
  if (object.type == ObjectType::Point) {
    const auto size = mapTileSize * 0.5f;
    const auto scaledSize = size * ratio;

    return {(object.x - (size.x / 2.0f)) * ratio.x,
            (object.y - (size.y / 2.0f)) * ratio.y,
            scaledSize.x,
            scaledSize.y};
  }
  else {
    return {object.x * ratio.x,
            object.y * ratio.y,
            object.width * ratio.x,
            object.height * ratio.y};
  }
}

}  // namespace

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
                 const Vector2f& pos) -> entt::entity
{
  const auto& info = ctx_get<comp::MapInfo>(registry);

  const auto [xRatio, yRatio] = get_viewport_scaling_ratio(registry);
  const Vector2f ratio{xRatio, yRatio};

  for (const auto objectEntity : layer.objects) {
    const auto& object = checked_get<comp::Object>(registry, objectEntity);
    const auto bounds = _get_hit_detection_bounds(object, info.tile_size, ratio);
    if (bounds.contains({pos.x, pos.y})) {
      return objectEntity;
    }
  }

  return entt::null;
}

}  // namespace tactile::sys
