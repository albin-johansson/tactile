/*
 * This source file is a part of the Tactile map editor.
 *
 * Copyright (C) 2022 Albin Johansson
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <https://www.gnu.org/licenses/>.
 */

#pragma once

#include <entt/entity/registry.hpp>

#include "core/common/ecs.hpp"
#include "core/components/attributes.hpp"
#include "core/components/layers.hpp"
#include "core/components/map_info.hpp"
#include "core/components/objects.hpp"
#include "core/components/parent.hpp"
#include "core/systems/context_system.hpp"
#include "core/systems/property_system.hpp"

namespace tactile::sys {

template <typename T>
auto deep_copy(entt::registry& registry,
               const entt::entity source,
               const entt::entity destination) -> T&
{
  return registry.emplace<T>(destination, registry.get<T>(source));
}

template <>
inline auto deep_copy<comp::Context>(entt::registry& registry,
                                              const entt::entity source,
                                              const entt::entity destination)
    -> comp::Context&
{
  auto& context = register_context(registry, destination);

  const auto& srcContext = registry.get<comp::Context>(source);
  context.name = srcContext.name;

  for (const auto srcPropertyEntity : srcContext.properties) {
    const auto propertyEntity = registry.create();
    context.properties.push_back(propertyEntity);

    deep_copy<comp::Property>(registry, srcPropertyEntity, propertyEntity);
  }

  return context;
}

template <>
inline auto deep_copy<comp::ObjectLayer>(entt::registry& registry,
                                         const entt::entity source,
                                         const entt::entity destination)
    -> comp::ObjectLayer&
{
  auto& map = ctx_get<comp::MapInfo>(registry);
  auto& layer = registry.emplace<comp::ObjectLayer>(destination);

  const auto& sourceLayer = registry.get<comp::ObjectLayer>(source);
  for (const auto sourceObject : sourceLayer.objects) {
    const auto objectEntity = registry.create();
    layer.objects.push_back(objectEntity);

    deep_copy<comp::Context>(registry, sourceObject, objectEntity);

    auto& object = deep_copy<comp::Object>(registry, sourceObject, objectEntity);
    object.id = map.next_object_id;
    ++map.next_object_id;
  }

  return layer;
}

}  // namespace tactile::sys
