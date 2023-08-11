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

#include <string>       // string
#include <string_view>  // string_view

#include "tactile/core/common/prelude.hpp"
#include "tactile/core/contexts/property_map.hpp"
#include "tactile/core/core.hpp"

namespace tactile {

class TACTILE_CORE_API PropertyBundle final {
 public:
  using const_iterator = PropertyMap::const_iterator;

  auto add(std::string name) -> Property&;

  auto remove(std::string_view name) -> Property;

  void update(std::string_view name, Property value);

  void rename(std::string_view current_name, std::string new_name);

  [[nodiscard]] auto get(std::string_view name) -> Property&;

  [[nodiscard]] auto get(std::string_view name) const -> const Property&;

  [[nodiscard]] auto has(std::string_view name) const -> bool;

  [[nodiscard]] auto size() const -> usize;

  [[nodiscard]] auto begin() const noexcept -> const_iterator;

  [[nodiscard]] auto end() const noexcept -> const_iterator;

 private:
  PropertyMap mProperties;
};

}  // namespace tactile
