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

#include <string>       // string
#include <string_view>  // string_view

#include <boost/uuid/uuid_hash.hpp>

#include "core/common/associative.hpp"
#include "core/common/ints.hpp"
#include "core/common/uuid.hpp"
#include "core/comp/component_definition.hpp"
#include "core/fwd.hpp"

namespace tactile {

/// Manages a set of component definitions.
class ComponentIndex final
{
 public:
  /**
   * Creates a new component definition.
   *
   * \param name a unique name.
   *
   * \return the identifier associated with the definition.
   *
   * \throws TactileError if the name is already used.
   */
  auto define_comp(std::string name) -> UUID;

  void restore_comp(ComponentDefinition def);

  /**
   * Removes an existing component definition.
   *
   * \param id the ID associated with the definition to remove.
   *
   * \throws TactileError if the identifier is invalid.
   */
  void remove_comp(const UUID& id);

  /**
   * Changes the name of an existing component.
   *
   * \param id the ID for the component that will be renamed.
   * \param name the new component name.
   *
   * \throws TactileError if the new name is already in use.
   */
  void rename_comp(const UUID& id, std::string name);

  /// Returns the component definition for a specific ID.
  [[nodiscard]] auto at(const UUID& id) -> ComponentDefinition&;
  [[nodiscard]] auto at(const UUID& id) const -> const ComponentDefinition&;

  [[nodiscard]] auto with_name(std::string_view name) -> ComponentDefinition&;

  [[nodiscard]] auto contains(const UUID& id) const -> bool;

  /// Indicates whether there is a component with a specific name.
  [[nodiscard]] auto contains(std::string_view name) const -> bool;

  /// Returns the amount of component definition.
  [[nodiscard]] auto size() const -> usize;

  [[nodiscard]] auto empty() const -> bool;

  [[nodiscard]] auto begin() const noexcept
  {
    return mDefs.begin();
  }

  [[nodiscard]] auto end() const noexcept
  {
    return mDefs.end();
  }

 private:
  HashMap<UUID, ComponentDefinition> mDefs;
};

}  // namespace tactile
