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

namespace tactile {

class PropertyBundle final
{
 public:
  void add(std::string name, AttributeType type);
  void add(std::string name, Attribute value);

  void update(std::string_view name, Attribute value);

  void remove(std::string_view name);

  void rename(std::string_view current, std::string updated);

  auto change_type(std::string_view name, AttributeType type) -> Attribute;

  [[nodiscard]] auto find(std::string_view name) -> Attribute*;
  [[nodiscard]] auto find(std::string_view name) const -> const Attribute*;

  [[nodiscard]] auto at(std::string_view name) -> Attribute&;
  [[nodiscard]] auto at(std::string_view name) const -> const Attribute&;

  [[nodiscard]] auto contains(std::string_view name) const -> bool;

  [[nodiscard]] auto size() const -> usize;

  [[nodiscard]] auto empty() const -> bool;

  [[nodiscard]] auto begin() const noexcept { return mProps.begin(); }
  [[nodiscard]] auto end() const noexcept { return mProps.end(); }

 private:
  TreeMap<std::string, Attribute> mProps;
};

}  // namespace tactile
