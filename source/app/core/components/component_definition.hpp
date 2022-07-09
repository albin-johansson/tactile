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
#include "core/common/associative.hpp"
#include "core/common/ints.hpp"
#include "core/common/uuid.hpp"
#include "core/components/component.hpp"
#include "core/element.hpp"

namespace tactile {

/// Represents the structure of a component.
class ComponentDefinition final : public IElement
{
 public:
  ComponentDefinition();

  explicit ComponentDefinition(const UUID& id);

  [[nodiscard]] auto instantiate() const -> Component;

  /**
   * Creates a new attribute.
   *
   * \param key the key to associate with the attribute.
   * \param type the initial type of the attribute.
   *
   * \throws TactileError if the key is already being used.
   */
  void add_attr(std::string key, AttributeType type = AttributeType::String);

  /**
   * Creates a new attribute.
   *
   * \param key the key to associate with the attribute.
   * \param value the initial value of the attribute.
   *
   * \throws TactileError if the key is already being used.
   */
  void add_attr(std::string key, Attribute value);

  /**
   * Updates the value of an existing attribute.
   *
   * The new value does not have to be of the same type that the attribute currently has.
   *
   * \param key the key associated with the attribute.
   * \param value the new attribute value.
   *
   * \throws TactileError if there is no attribute for the key.
   */
  void update_attr(std::string_view key, Attribute value);

  /**
   * Removes an existing attribute.
   *
   * \param key the key associated with the attribute.
   *
   * \throws TactileError if there is no attribute for the key.
   */
  void remove_attr(std::string_view key);

  /**
   * Changes the name (key) of an existing attribute.
   *
   * \param current the current attribute key.
   * \param updated the new attribute key.
   *
   * \throws TactileError if there is no attribute for the current key.
   * \throws TactileError if the updated key is already being used.
   */
  void rename_attr(std::string_view current, std::string updated);

  /**
   * Duplicates an existing attribute.
   *
   * \param key the key associated with the attribute that will be duplicated.
   *
   * \return the key of the created attribute.
   */
  auto duplicate_attr(std::string_view key) -> std::string;

  /// Returns the value of the attribute for a specific key.
  [[nodiscard]] auto get_attr(std::string_view key) const -> const Attribute&;

  /// Indicates whether there is an attribute for a specific key.
  [[nodiscard]] auto has_attr(std::string_view key) const -> bool;

  void set_name(std::string name);

  [[nodiscard]] auto get_name() const -> const std::string&;

  /// Returns the amount of attributes in the component.
  [[nodiscard]] auto size() const -> usize;

  /// Indicates whether the component has no attributes.
  [[nodiscard]] auto empty() const -> bool;

  [[nodiscard]] auto get_uuid() const -> const UUID& override;

  [[nodiscard]] auto begin() const noexcept { return mAttributes.begin(); }
  [[nodiscard]] auto end() const noexcept { return mAttributes.end(); }

 private:
  UUID                            mId {make_uuid()};
  std::string                     mName;
  TreeMap<std::string, Attribute> mAttributes;
};

}  // namespace tactile
