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

#include "common/type/hash_map.hpp"
#include "common/type/result.hpp"
#include "common/type/string.hpp"
#include "common/type/uuid.hpp"
#include "core/component/component_definition.hpp"

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
  auto remove_comp(const UUID& component_id) -> Result;

  /// Changes the name of an existing component.
  ///
  /// \param component_id the ID of the component that will be renamed.
  /// \param name the new component name, must not be used by another component.
  /// \return success if the name was unique and the ID was valid; failure otherwise.
  auto rename_comp(const UUID& component_id, String name) -> Result;

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
