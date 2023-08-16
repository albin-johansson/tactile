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

#include "tactile/core/api.hpp"
#include "tactile/core/common/prelude.hpp"
#include "tactile/core/contexts/component_bundle.hpp"
#include "tactile/core/contexts/property_bundle.hpp"

namespace tactile {

class TACTILE_CORE_API Metadata final {
 public:
  void set_name(std::string name);

  [[nodiscard]] auto get_name() const -> const std::string&;

  [[nodiscard]] auto get_props() -> PropertyBundle&;
  [[nodiscard]] auto get_props() const -> const PropertyBundle&;

  [[nodiscard]] auto get_comps() -> ComponentBundle&;
  [[nodiscard]] auto get_comps() const -> const ComponentBundle&;

 private:
  std::string mName;
  PropertyBundle mProperties;
  ComponentBundle mComponents;
};

}  // namespace tactile
