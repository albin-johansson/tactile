// Copyright (C) 2023 Albin Johansson (GNU General Public License v3.0)

#pragma once

#include <concepts>  // invocable

#include "tactile/core/api.hpp"
#include "tactile/core/meta/component.hpp"
#include "tactile/foundation/container/hash_map.hpp"
#include "tactile/foundation/container/string.hpp"
#include "tactile/foundation/functional/maybe.hpp"
#include "tactile/foundation/misc/uuid.hpp"
#include "tactile/foundation/prelude.hpp"

namespace tactile::core {

/**
 * Represents a component definition.
 */
struct ComponentDefinition final {
  String name;          ///< The component name.
  Component prototype;  ///< Provides the attribute names and their default values.
};

/**
 * Represents a collection of component definitions associated with a document.
 *
 * \details
 *    Each component in a component set features a unique user-provided name that serves as the
 *    sole identifier from the point-of-view of a user. However, UUIDs are used as the primary
 *    identification method internally. These UUIDs are not persistent and shall not be used in
 *    save files or any user facing views.
 */
class TACTILE_CORE_API ComponentSet final {
 public:
  /**
   * Defines a new empty component.
   *
   * \note
   *    This function has no effect if the specified name is already in use by another component.
   *
   * \param name The component name.
   *
   * \return
   *    The UUID of the new component.
   */
  auto define_component(String name) -> Maybe<UUID>;

  /**
   * Removes a component from the set.
   *
   * \param uuid The UUID of the desired component.
   *
   * \return
   *    The removed component definition.
   */
  auto remove_component(const UUID& uuid) -> Maybe<ComponentDefinition>;

  /**
   * Attempts to find a component with a given name.
   *
   * \param name The name of the desired component.
   *
   * \return
   *    The UUID of the found component.
   */
  [[nodiscard]]
  auto get_component_with_name(StringView name) const -> Maybe<UUID>;

  /**
   * Returns a component stored in the set.
   *
   * \param uuid The UUID of the desired component.
   *
   * \return
   *    A component.
   */
  [[nodiscard]]
  auto get_component(const UUID& uuid) -> Component&;

  /** \copydoc get_component() */
  [[nodiscard]]
  auto get_component(const UUID& uuid) const -> const Component&;

  /**
   * Indicates whether the set has a component with a given UUID.
   *
   * \param uuid The component UUID to look for.
   *
   * \return
   *    True if a component was found; false otherwise.
   */
  [[nodiscard]]
  auto has_component(const UUID& uuid) const -> bool;

  /**
   * Returns the number of components in the set.
   *
   * \return
   *    A component count.
   */
  [[nodiscard]]
  auto component_count() const -> ssize;

  /**
   * Invokes a callback for each component in the set.
   *
   * \param callable The function object.
   */
  template <std::invocable<const ComponentDefinition&> T>
  void each(const T& callable) const
  {
    for (const auto& [uuid, definition] : mComponents) {
      callable(definition);
    }
  }

 private:
  using ComponentDefinitionMap = HashMap<UUID, ComponentDefinition>;

  ComponentDefinitionMap mComponents {};
};

}  // namespace tactile::core
