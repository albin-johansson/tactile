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
#include "core/common/vocabulary.hpp"
#include "core/type/string.hpp"
#include "core/type/tree_map.hpp"

namespace tactile {

class PropertyBundle final {
 public:
  void add(String name, AttributeType type);
  void add(String name, Attribute value);

  void update(StringView name, Attribute value);

  void remove(StringView name);

  void rename(StringView current, String updated);

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
  TreeMap<String, Attribute> mProps;
};

}  // namespace tactile
