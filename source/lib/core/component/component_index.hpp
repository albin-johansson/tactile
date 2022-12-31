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

#include <boost/uuid/uuid_hash.hpp>

#include "core/component/component_definition.hpp"
#include "common/type/hash_map.hpp"
#include "common/type/string.hpp"
#include "common/type/uuid.hpp"
#include "core/vocabulary.hpp"

namespace tactile {

/// Manages a set of component definitions.
class ComponentIndex final {
 public:
  /// Creates a new component definition.
  ///
  /// \param name a unique name.
  ///
  /// \return the identifier associated with the definition.
  auto define(String name) -> UUID;

  /// Restores a previously removed component definition.
  void restore(ComponentDefinition def);

  /// Removes an existing component definition.
  ///
  /// \param id the ID associated with the definition to remove.
  void remove(const UUID& id);

  /// Changes the name of an existing component.
  ///
  /// \param id the ID for the component that will be renamed.
  /// \param name the new component name.
  void rename(const UUID& id, String name);

  /// Returns the component definition for a specific ID.
  [[nodiscard]] auto at(const UUID& id) -> ComponentDefinition&;
  [[nodiscard]] auto at(const UUID& id) const -> const ComponentDefinition&;

  /// Returns the component definition with a specific name.
  [[nodiscard]] auto with_name(StringView name) -> ComponentDefinition&;

  /// Indicates whether there is a component definition with a specific ID.
  [[nodiscard]] auto contains(const UUID& id) const -> bool;

  /// Indicates whether there is a component with a specific name.
  [[nodiscard]] auto contains(StringView name) const -> bool;

  /// Returns the amount of component definition.
  [[nodiscard]] auto size() const -> usize;

  [[nodiscard]] auto empty() const -> bool;

  [[nodiscard]] auto begin() const noexcept { return mDefs.begin(); }
  [[nodiscard]] auto end() const noexcept { return mDefs.end(); }

 private:
  HashMap<UUID, ComponentDefinition> mDefs;
};

}  // namespace tactile
