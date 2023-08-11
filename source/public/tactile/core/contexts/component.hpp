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
#include "tactile/core/contexts/property_bundle.hpp"
#include "tactile/core/core.hpp"

namespace tactile {

struct TACTILE_CORE_API ComponentInstance final {
  ComponentID type_id;
  PropertyBundle attributes;
};

class TACTILE_CORE_API Component final {
 public:
  Component(ComponentID id, std::string name);

  void set_name(std::string name);

  auto add_attribute(std::string name) -> Property&;

  void remove_attribute(std::string_view name);

  void rename_attribute(std::string_view current_name, std::string new_name);

  void update_attribute(std::string_view name, Property value);

  [[nodiscard]] auto instantiate() const -> ComponentInstance;

  [[nodiscard]] auto get_id() const -> ComponentID;

  [[nodiscard]] auto get_name() const -> const std::string&;

 private:
  ComponentID mID;
  std::string mName;
  PropertyBundle mAttributes;
};

}  // namespace tactile
