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

#pragma once

#include <utility>  // forward

#include <entt/entity/registry.hpp>

#include "core/common/ecs.hpp"
#include "core/common/functional.hpp"

namespace tactile {

/**
 * \brief A lightweight wrapper around an entity along with its parent registry.
 *
 * \details This experimental class is designed to simplify some system function
 * signatures. It is very cheap to create instances of this class, and actors are not
 * meant to be stored for long periods of time. Instead, they are intended to be used in
 * function signatures and implementations to clarify ownership. Fundamentally, this class
 * attempts to make code easier to read.
 *
 * \code{cpp}
 * void foo(const Actor& actor)
 * {
 *   if (actor.has<SomeComponent>()) {
 *     const auto& component = actor.get<SomeComponent>();
 *     // ...
 *   }
 *
 *   if (const auto* other = actor.find<OtherComponent>()) {
 *     // ...
 *   }
 * }
 * \endcode
 *
 * \ingroup core
 */
class Actor final
{
 public:
  /**
   * \brief Creates an actor.
   *
   * \param registry the host registry.
   * \param entity a valid entity identifier.
   *
   * \throws TactileError if the entity is invalid.
   */
  Actor(entt::registry& registry, entt::entity entity);

  /// Creates an actor that wraps a new entity in a registry
  [[nodiscard]] static auto of(entt::registry& registry) -> Actor;

  template <typename Component, typename... Args>
  auto add(Args&&... args) -> Component&
  {
    return registry().emplace_or_replace<Component>(mEntity, std::forward<Args>(args)...);
  }

  template <typename Component>
  [[nodiscard]] auto get() -> Component&
  {
    return checked_get<Component>(registry(), mEntity);
  }

  template <typename Component>
  [[nodiscard]] auto get() const -> const Component&
  {
    return checked_get<Component>(registry(), mEntity);
  }

  template <typename Component>
  [[nodiscard]] auto find() -> Component*
  {
    return registry().try_get<Component>(mEntity);
  }

  template <typename Component>
  [[nodiscard]] auto find() const -> const Component*
  {
    return registry().try_get<Component>(mEntity);
  }

  template <typename Component>
  [[nodiscard]] auto has() const -> bool
  {
    return registry().all_of<Component>(mEntity);
  }

  [[nodiscard]] auto registry() -> entt::registry& { return mRegistry.get(); }
  [[nodiscard]] auto registry() const -> const entt::registry& { return mRegistry.get(); }

  [[nodiscard]] auto entity() const noexcept -> entt::entity { return mEntity; }

 private:
  Ref<entt::registry> mRegistry;
  entt::entity mEntity{entt::null};
};

}  // namespace tactile