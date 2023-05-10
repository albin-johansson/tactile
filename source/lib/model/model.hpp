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

#pragma once

#include "common/debug/assert.hpp"
#include "common/type/ecs.hpp"

namespace tactile {

class Model final {
 public:
  [[nodiscard]] auto create_entity() -> Entity;

  void destroy(Entity entity);

  void set_enabled(Entity entity, bool enabled);

  [[nodiscard]] auto is_enabled(Entity entity) const -> bool;

  template <typename T>
  auto add() -> T&
  {
    TACTILE_ASSERT(!mRegistry.ctx().contains<T>());
    return mRegistry.ctx().emplace<T>();
  }

  template <typename T>
  auto add(const Entity entity) -> T&
  {
    TACTILE_ASSERT(is_enabled(entity));
    TACTILE_ASSERT(!mRegistry.all_of<T>(entity));
    return mRegistry.emplace<T>(entity);
  }

  template <typename T>
  void remove(const Entity entity)
  {
    TACTILE_ASSERT(is_enabled(entity));
    mRegistry.remove<T>(entity);
  }

  template <typename T>
  [[nodiscard]] auto get(const Entity entity) -> T&
  {
    TACTILE_ASSERT(is_enabled(entity));
    TACTILE_ASSERT(mRegistry.all_of<T>(entity));
    return mRegistry.get<T>(entity);
  }

  template <typename T>
  [[nodiscard]] auto get(const Entity entity) const -> const T&
  {
    TACTILE_ASSERT(is_enabled(entity));
    TACTILE_ASSERT(mRegistry.all_of<T>(entity));
    return mRegistry.get<T>(entity);
  }

  template <typename T>
  [[nodiscard]] auto try_get(const Entity entity) -> T*
  {
    TACTILE_ASSERT(is_enabled(entity));
    return mRegistry.try_get<T>(entity);
  }

  template <typename T>
  [[nodiscard]] auto try_get(const Entity entity) const -> const T*
  {
    TACTILE_ASSERT(is_enabled(entity));
    return mRegistry.try_get<T>(entity);
  }

  template <typename T>
  [[nodiscard]] auto unchecked_try_get(const Entity entity) -> T*
  {
    return (entity != kNullEntity) ? mRegistry.try_get<T>(entity) : nullptr;
  }

  template <typename T>
  [[nodiscard]] auto unchecked_try_get(const Entity entity) const -> const T*
  {
    return (entity != kNullEntity) ? mRegistry.try_get<T>(entity) : nullptr;
  }

  template <typename T>
  [[nodiscard]] auto get() -> T&
  {
    TACTILE_ASSERT(mRegistry.ctx().contains<T>());
    return mRegistry.ctx().get<T>();
  }

  template <typename T>
  [[nodiscard]] auto get() const -> const T&
  {
    TACTILE_ASSERT(mRegistry.ctx().contains<T>());
    return mRegistry.ctx().get<T>();
  }

  template <typename T>
  [[nodiscard]] auto has(const Entity entity) const -> bool
  {
    TACTILE_ASSERT(is_enabled(entity));
    return mRegistry.all_of<T>(entity);
  }

  template <typename... Components>
  [[nodiscard]] auto each()
  {
    // return mRegistry.view<Components...>(entt::exclude<DisabledTag>).each();
    return mRegistry.view<Components...>().each();
  }

  template <typename... Components>
  [[nodiscard]] auto each() const
  {
    // return mRegistry.view<Components...>(entt::exclude<DisabledTag>).each();
    return mRegistry.view<Components...>().each();
  }

 private:
  Registry mRegistry;
};

/// Returns the global model instance.
[[nodiscard]] auto get_model() -> Model&;
[[nodiscard]] auto get_global_model() -> Model&;

}  // namespace tactile
