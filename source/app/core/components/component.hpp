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

#include <functional>  // less
#include <map>         // map
#include <string>      // string
#include <vector>      // vector

#include <entt/entt.hpp>

#include "core/attribute.hpp"
#include "tactile.hpp"

namespace tactile {

using component_attribute_map = std::map<std::string, attribute_value, std::less<>>;

namespace comp {

/**
 * Describes the structure of a component.
 *
 * Component definitions are unique to each map (for now).
 *
 * \todo A possible use case for projects could be to provide common component
 * definitions.
 *
 * \todo Should it be possible to import component definitions from existing maps?
 */
struct component_def final
{
  component_id id{};  ///< Unique ID for the component definition.
  std::string name;   ///< The human-readable component name, also unique.
  component_attribute_map attributes;
};

/**
 * Represents a user-defined component that can be attached to attribute context entities.
 */
struct component final
{
  component_id type;               ///< The ID of the component definition type.
  component_attribute_map values;  ///< Current values.
};

}  // namespace comp
}  // namespace tactile