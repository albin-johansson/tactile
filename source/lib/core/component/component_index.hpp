// Copyright (C) 2024 Albin Johansson (GNU General Public License v3.0)

#pragma once

#include "core/component/component_definition.hpp"
#include "tactile/base/container/expected.hpp"
#include "tactile/base/container/hash_map.hpp"
#include "tactile/base/container/string.hpp"
#include "tactile/core/util/uuid.hpp"

namespace tactile {

/// Manages a set of component definitions.
class ComponentIndex final {
 public:
  /// Creates a new component definition.
  ///
  /// \param name a unique component name.
  /// \return the identifier assigned the new component definition.
  auto define_comp(String name) -> UUID;

  /// Restores a component definition that was previously removed.
  ///
  /// \param component_def a valid but unmanaged component definition.
  void restore_comp(ComponentDefinition component_def);

  /// Removes an existing component definition.
  ///
  /// \param component_id the ID of the component to remove.
  /// \return success if a component was removed; failure otherwise.
  auto remove_comp(const UUID& component_id) -> Result<void>;

  /// Changes the name of an existing component.
  ///
  /// \param component_id the ID of the component that will be renamed.
  /// \param name the new component name, must not be used by another component.
  /// \return success if the name was unique and the ID was valid; failure otherwise.
  auto rename_comp(const UUID& component_id, String name) -> Result<void>;

  /// Returns the component definition with the given ID.
  [[nodiscard]] auto get_comp(const UUID& component_id) -> ComponentDefinition&;
  [[nodiscard]] auto get_comp(const UUID& component_id) const
      -> const ComponentDefinition&;

  /// Returns the component definition with a specific name.
  [[nodiscard]] auto get_comp(StringView name) -> ComponentDefinition&;

  /// Indicates whether there is a component definition with a specific ID.
  [[nodiscard]] auto has_comp(const UUID& component_id) const -> bool;

  /// Indicates whether there is a component with a specific name.
  [[nodiscard]] auto has_comp(StringView name) const -> bool;

  /// Returns the amount of component definition.
  [[nodiscard]] auto comp_count() const -> usize;

  [[nodiscard]] auto empty() const -> bool;

  [[nodiscard]] auto begin() const noexcept { return mDefs.begin(); }
  [[nodiscard]] auto end() const noexcept { return mDefs.end(); }

 private:
  HashMap<UUID, ComponentDefinition> mDefs;
};

}  // namespace tactile
