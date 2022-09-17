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

#include "core/attribute.hpp"
#include "core/common/assoc.hpp"
#include "core/common/uuid.hpp"
#include "core/common/vocabulary.hpp"

namespace tactile {

/// Represents an instance of a component definition.
class Component final {
 public:
  using AttributeMap = TreeMap<std::string, Attribute>;

  /// Creates a component.
  ///
  /// \param definition_id the ID of a component definition.
  /// \param attributes the default attribute values.
  Component(const UUID& definition_id, AttributeMap attributes);

  /// Adds a new attribute to the component.
  ///
  /// This is only used on attached components after the component definition is modified.
  ///
  /// \param key the name of the attribute.
  /// \param value the default value of the attribute.
  void add_attr(std::string key, Attribute value);

  /// Removes an attribute from the component.
  ///
  /// This is only used on attached components after the component definition is modified.
  ///
  /// \param key the name of the attribute.
  void remove_attr(std::string_view key);

  /// Changes the value of an attribute.
  ///
  /// \param key the name of the attribute.
  /// \param value the new value of the attribute.
  void update_attr(std::string_view key, Attribute value);

  /// Changes the name of an attribute.
  ///
  /// This is only used on attached components after the component definition is modified.
  ///
  /// \param old_key the current name of the attribute.
  /// \param new_key the new name of the attribute.
  void rename_attr(std::string_view old_key, std::string new_key);

  /// Returns the value of an attribute.
  [[nodiscard]] auto get_attr(std::string_view key) const -> const Attribute&;

  /// Indicates whether the component has an attribute with a specific name.
  [[nodiscard]] auto has_attr(std::string_view key) const -> bool;

  /// Returns the amount of attributes in the component.
  [[nodiscard]] auto size() const -> usize;

  /// Indicates whether the component has no attributes.
  [[nodiscard]] auto empty() const -> bool;

  /// Returns the ID of the associated component definition type.
  [[nodiscard]] auto definition_id() const -> const UUID& { return mDefinitionId; }

  [[nodiscard]] auto begin() const noexcept { return mAttributes.begin(); }
  [[nodiscard]] auto end() const noexcept { return mAttributes.end(); }

 private:
  UUID mDefinitionId;
  AttributeMap mAttributes;
};

}  // namespace tactile
