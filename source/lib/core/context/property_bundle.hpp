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

#include "core/attribute.hpp"
#include "common/type/string.hpp"
#include "common/type/string_map.hpp"
#include "core/vocabulary.hpp"

namespace tactile {

/// Represents a collection of properties attached to a context.
class PropertyBundle final {
 public:
  /// Adds a new property using the default value for the specified type.
  ///
  /// \param name the name to associate with the property, must be unique.
  /// \param type the initial type of the property.
  void add(String name, AttributeType type);

  /// Adds a new property with an initial value.
  ///
  /// \param name the name to associate with the property, must be unique.
  /// \param value the initial value of the property.
  void add(String name, Attribute value);

  /// Updates the value of an existing property.
  ///
  /// \param name the name of the property.
  /// \param value the new value of the property.
  void update(StringView name, Attribute value);

  /// Removes a property.
  ///
  /// \param name the name of the property to remove.
  /// \returns true if a property was removed; false otherwise.
  auto remove(StringView name) -> bool;

  /// Changes the name associated with a property.
  ///
  /// \param current the current name of the property.
  /// \param updated the new name of the property, cannot be in use by another property.
  /// \returns true if a property was renamed; false otherwise.
  auto rename(StringView current, String updated) -> bool;

  /// Changes the type of an existing property, assuming the default value for the type.
  ///
  /// \param name the name of the property that will be modified.
  /// \param type the new property type.
  /// \return the previous value of the property.
  auto change_type(StringView name, AttributeType type) -> Attribute;

  [[nodiscard]] auto find(StringView name) -> Attribute*;
  [[nodiscard]] auto find(StringView name) const -> const Attribute*;

  [[nodiscard]] auto at(StringView name) -> Attribute&;
  [[nodiscard]] auto at(StringView name) const -> const Attribute&;

  [[nodiscard]] auto contains(StringView name) const -> bool;

  [[nodiscard]] auto size() const -> usize;

  [[nodiscard]] auto empty() const -> bool;

  [[nodiscard]] auto begin() const noexcept { return mProps.begin(); }
  [[nodiscard]] auto end() const noexcept { return mProps.end(); }

 private:
  StringMap<Attribute> mProps;
};

}  // namespace tactile
