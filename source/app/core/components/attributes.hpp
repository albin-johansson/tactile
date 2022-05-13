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

#include <entt/entity/entity.hpp>

#include "core/attribute.hpp"
#include "core/common/associative.hpp"
#include "tactile.hpp"

namespace tactile {

using ComponentAttrMap = TreeMap<std::string, Attribute>;

}  // namespace tactile

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
  ContextID id{};    ///< The unique context identifier (not persistent).
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

/**
 * \brief Component that represents a named attribute value.
 *
 * \ingroup components
 */
struct Property final
{
  std::string name;  ///< The name that is unique within the associated context.
  Attribute value;   ///< The property attribute value.
};

/**
 * \brief Describes the structure of a component.
 *
 * \details Component definitions are unique to each map (for now).
 *
 * \todo A possible use case for projects could be to provide common component
 * definitions.
 *
 * \todo Should it be possible to import component definitions from existing maps?
 *
 * \ingroup components
 */
struct ComponentDef final
{
  ComponentID id{};             ///< Unique ID for the component definition.
  std::string name;             ///< The component name (which is unique!).
  ComponentAttrMap attributes;  ///< The attributes with their default values.
};

/**
 * \brief Represents a user-defined bundle of properties.
 *
 * \details Components are supported by all attribute contexts.
 *
 * \ingroup components
 */
struct Component final
{
  ComponentID type{};       ///< The ID of the component definition type.
  ComponentAttrMap values;  ///< Current values.
};

}  // namespace tactile::comp
