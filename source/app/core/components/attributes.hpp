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

#include <string>  // string
#include <vector>  // vector

#include <entt/entt.hpp>

#include "tactile.hpp"

namespace tactile::comp {

/**
 * \brief Featured by all entities that can host properties and components.
 *
 * \details This component is featured by many different kinds of items, such as:
 * - Maps
 * - Layers
 * - Tilesets
 * - Tiles
 * - Layer objects
 *
 * \ingroup components
 */
struct AttributeContext final
{
  context_id id{};   ///< The unique context identifier (not persistent).
  std::string name;  ///< The name of the context.
  std::vector<entt::entity> properties;  ///< All associated properties.
  std::vector<entt::entity> components;  ///< All associated components.
};

/**
 * \brief Context component that keeps track of the current attribute context.
 *
 * \details There is always an active attribute context, a null active entity is used to
 * indicate that the root map is the active context.
 *
 * \ingroup components
 */
struct ActiveAttributeContext final
{
  entt::entity entity{entt::null};  ///< The current context entity (may be null).
};

}  // namespace tactile::comp
