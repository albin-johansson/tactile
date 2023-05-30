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

#include <concepts>  // predicate
#include <utility>   // forward

#include "common/debug/assert.hpp"
#include "common/type/ecs.hpp"

namespace tactile {

/// A wrapper around an entity registry.
class Model final {
 public:
  /// Creates a new entity with no attached components.
  [[nodiscard]] auto create_entity() -> Entity;

  /**
   * Destroys an entity.
   *
   * \details This function does nothing if the null entity is provided.
   *
   * \param entity a valid entity.
   */
  void destroy(Entity entity);

  /**
   * Sets whether an entity is enabled.
   *
   * \param entity a valid entity.
   * \param enabled true if the entity should be enabled; false otherwise.
   */
  void set_enabled(Entity entity, bool enabled);

  [[nodiscard]] auto is_enabled(Entity entity) const -> bool;

  /**
   * Sorts a component pool using a sorter predicate.
   *
   * \tparam T a component type.
   *
   * \param sorter the function object used to sort components.
   */
  template <typename T, std::predicate<const T&, const T&> Predicate>
  void sort(Predicate&& sorter)
  {
    mRegistry.sort<T>(std::forward<Predicate>(sorter));
  }

  /**
   * Adds a context component to the model and returns a reference to it.
   *
   * \note The existing context component is returned if the model already has a context
   * component of the specified type.
   *
   * \tparam T a component type.
   * \return
   */
  template <typename T>
  auto add() -> T&
  {
    return mRegistry.ctx().emplace<T>();
  }

  /**
   * Adds (or replaces) a component to an entity.
   *
   * \pre the entity must be enabled.
   *
   * \tparam T a component type.
   * \param entity a valid entity.
   */
  template <typename T>
  auto add(const Entity entity) -> T&
  {
    TACTILE_ASSERT(is_enabled(entity));
    return mRegistry.emplace_or_replace<T>(entity);
  }

  /**
   * Removes a component from an entity.
   *
   * \pre the entity must be enabled.
   *
   * \tparam T a component type.
   * \param entity a valid entity.
   */
  template <typename T>
  void remove(const Entity entity)
  {
    TACTILE_ASSERT(is_enabled(entity));
    mRegistry.remove<T>(entity);
  }

  /**
   * Returns a component attached to an entity.
   *
   * \pre the entity must be enabled.
   * \pre the entity must feature the component.
   *
   * \tparam T a component type.
   * \param entity a valid entity.
   *
   * \return a reference to the component.
   */
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

  /// Returns a component attached to an entity, or null if it doesn't exist.
  template <typename T>
  [[nodiscard]] auto try_get(const Entity entity) -> T*
  {
    TACTILE_ASSERT(is_enabled(entity));
    return mRegistry.try_get<T>(entity);
  }

  /// Returns a component attached to an entity, or null if it doesn't exist.
  template <typename T>
  [[nodiscard]] auto try_get(const Entity entity) const -> const T*
  {
    TACTILE_ASSERT(is_enabled(entity));
    return mRegistry.try_get<T>(entity);
  }

  /// Returns a component attached to a entity, or null if it doesn't exist.
  /// \note This function can safely be used with invalid entity identifiers.
  template <typename T>
  [[nodiscard]] auto unchecked_try_get(const Entity entity) -> T*
  {
    return (entity != kNullEntity) ? mRegistry.try_get<T>(entity) : nullptr;
  }

  /// Returns a component attached to a entity, or null if it doesn't exist.
  /// \note This function can safely be used with invalid entity identifiers.
  template <typename T>
  [[nodiscard]] auto unchecked_try_get(const Entity entity) const -> const T*
  {
    return (entity != kNullEntity) ? mRegistry.try_get<T>(entity) : nullptr;
  }

  /// Returns a context component.
  template <typename T>
  [[nodiscard]] auto get() -> T&
  {
    TACTILE_ASSERT(mRegistry.ctx().contains<T>());
    return mRegistry.ctx().get<T>();
  }

  /// Returns a context component.
  template <typename T>
  [[nodiscard]] auto get() const -> const T&
  {
    TACTILE_ASSERT(mRegistry.ctx().contains<T>());
    return mRegistry.ctx().get<T>();
  }

  /// Indicates whether the model has a specific context component.
  template <typename T>
  [[nodiscard]] auto has() const -> bool
  {
    return mRegistry.ctx().contains<T>();
  }

  /// Indicates whether an entity has a specific component.
  template <typename T>
  [[nodiscard]] auto has(const Entity entity) const -> bool
  {
    TACTILE_ASSERT(is_enabled(entity));
    return mRegistry.all_of<T>(entity);
  }

  /// Returns an iterable view of all entities featuring the specified components.
  /// \note This function includes all entities, including disabled entities.
  template <typename... Components>
  [[nodiscard]] auto each()
  {
    return mRegistry.view<Components...>().each();
  }

  /// Returns an iterable view of all entities featuring the specified components.
  /// \note This function includes all entities, including disabled entities.
  template <typename... Components>
  [[nodiscard]] auto each() const
  {
    return mRegistry.view<Components...>().each();
  }

 private:
  Registry mRegistry;
};

}  // namespace tactile
