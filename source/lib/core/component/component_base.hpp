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

#include "common/type/result.hpp"
#include "common/type/string_map.hpp"
#include "core/attribute.hpp"
#include "tactile/base/container/string.hpp"
#include "tactile/base/int.hpp"
#include "tactile/core/util/uuid.hpp"

namespace tactile {

/// Provides the common API for component definitions and their instances.
class ComponentBase {
 public:
  using AttributeMap = StringMap<Attribute>;

  explicit ComponentBase(const UUID& type);

  /// Creates a new attribute.
  ///
  /// \param key the key to associate with the attribute.
  /// \param type the initial type of the attribute.
  void add_attr(String key, AttributeType type = AttributeType::String);

  /// Creates a new attribute.
  ///
  /// \param key the key to associate with the attribute.
  /// \param value the initial value of the attribute.
  void add_attr(String key, Attribute value);

  /// Updates the value of an existing attribute.
  ///
  /// \details
  /// The new value does not have to be of the same type as the old value.
  ///
  /// \param key the key associated with the attribute.
  /// \param value the new attribute value.
  void update_attr(StringView key, Attribute value);

  /// Removes an existing attribute.
  ///
  /// \param key the key associated with the attribute.
  /// \return success if an attribute was removed; failure otherwise.
  auto remove_attr(StringView key) -> Result;

  /// Changes the name (key) of an existing attribute.
  ///
  /// \param old_key the current attribute key.
  /// \param new_key the new attribute key.
  /// \return success if an attribute was renamed; failure otherwise.
  auto rename_attr(StringView old_key, String new_key) -> Result;

  /// Duplicates an existing attribute.
  ///
  /// \param key the name of the attribute to duplicate.
  /// \return the key associated with the new attribute.
  auto duplicate_attr(StringView key) -> String;

  /// Returns the value of an attribute for a specific key.
  [[nodiscard]] auto get_attr(StringView key) const -> const Attribute&;

  /// Indicates whether there is an attribute for a specific key.
  [[nodiscard]] auto has_attr(StringView key) const -> bool;

  /// Returns the amount of attributes in the component.
  [[nodiscard]] auto attr_count() const -> usize;

  /// Indicates whether the component has no attributes.
  [[nodiscard]] auto empty() const -> bool;

  [[nodiscard]] auto begin() const noexcept { return mAttributes.begin(); }
  [[nodiscard]] auto end() const noexcept { return mAttributes.end(); }

 protected:
  UUID mTypeId;
  AttributeMap mAttributes;
};

}  // namespace tactile
