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
#include "core/type/string.hpp"
#include "core/type/string_map.hpp"
#include "core/uuid.hpp"
#include "core/vocabulary.hpp"

namespace tactile {

/// Provides the common API for component definitions and instances.
class ComponentBase {
 public:
  using AttributeMap = StringMap<Attribute>;

  explicit ComponentBase(const UUID& type);

  /// Creates a new attribute.
  ///
  /// \param key the key to associate with the attribute.
  /// \param type the initial type of the attribute.
  void add(String key, AttributeType type = AttributeType::String);

  /// Creates a new attribute.
  ///
  /// \param key the key to associate with the attribute.
  /// \param value the initial value of the attribute.
  void add(String key, Attribute value);

  /// Updates the value of an existing attribute.
  ///
  /// The new value does not have to be of the same type that the attribute currently has.
  ///
  /// \param key the key associated with the attribute.
  /// \param value the new attribute value.
  void update(StringView key, Attribute value);

  /// Removes an existing attribute (returning true if an attribute was removed).
  ///
  /// \param key the key associated with the attribute.
  auto remove(StringView key) -> bool;

  /// Changes the name (key) of an existing attribute.
  ///
  /// The new attribute name must be unique.
  ///
  /// \param current the current attribute key.
  /// \param updated the new attribute key.
  ///
  /// \return true if an attribute was renamed; false otherwise.
  auto rename(StringView current, String updated) -> bool;

  /// Duplicates an existing attribute.
  auto duplicate(StringView key) -> String;

  /// Returns the value of the attribute for a specific key.
  [[nodiscard]] auto at(StringView key) const -> const Attribute&;

  /// Indicates whether there is an attribute for a specific key.
  [[nodiscard]] auto has(StringView key) const -> bool;

  /// Returns the amount of attributes in the component.
  [[nodiscard]] auto size() const -> usize;

  /// Indicates whether the component has no attributes.
  [[nodiscard]] auto empty() const -> bool;

  [[nodiscard]] auto begin() const noexcept { return mAttributes.begin(); }
  [[nodiscard]] auto end() const noexcept { return mAttributes.end(); }

 protected:
  UUID mTypeId;
  AttributeMap mAttributes;
};

}  // namespace tactile
