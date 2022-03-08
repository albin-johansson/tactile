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

#include <entt/entt.hpp>

#include "core/attribute.hpp"
#include "core/components/attribute_context.hpp"
#include "core/components/property.hpp"
#include "snapshot.hpp"
#include "tactile.hpp"

namespace tactile::sys {

/**
 * \ingroup systems
 * \defgroup property-system Property System
 */

/// \addtogroup property-system
/// \{

/// \name Property system
/// \{

/**
 * \brief Adds a property to an attribute context.
 *
 * \pre The supplied name must be unique within the attribute context.
 *
 * \param registry the document registry.
 * \param context the context that will be modified.
 * \param name the name of the property.
 * \param type the type of the property value.
 */
void add_property(entt::registry& registry,
                  comp::attribute_context& context,
                  std::string name,
                  attribute_type type);

/**
 * \brief Adds a property to an attribute context.
 *
 * \pre The name must be unique within the attribute context.
 *
 * \param registry the document registry.
 * \param context the context that will be modified.
 * \param name the name of the property.
 * \param value the initial value of the property.
 */
void add_property(entt::registry& registry,
                  comp::attribute_context& context,
                  std::string name,
                  attribute_value value);

/**
 * \brief Removes a property from an attribute context.
 *
 * \pre The context must feature a property with the specified name.
 *
 * \param registry the document registry.
 * \param context the context that will be modified.
 * \param name the name of the property that will be removed.
 */
void remove_property(entt::registry& registry,
                     comp::attribute_context& context,
                     std::string_view name);

/**
 * \brief Renames a property in an attribute context.
 *
 * \pre The context must feature a property with the _old_ name.
 * \pre The context must _not_ feature a property with the _new_ name.
 *
 * \param registry the document registry.
 * \param context the context that will be modified.
 * \param oldName the name of the property that will be renamed.
 * \param newName the new name of the property.
 */
void rename_property(entt::registry& registry,
                     comp::attribute_context& context,
                     std::string_view oldName,
                     std::string newName);

/**
 * \brief Updates the value of an existing property.
 *
 * \pre The context must feature a property with the specified name.
 *
 * \param registry the document registry.
 * \param context the context that will be modified.
 * \param name the name of the property that will be updated.
 * \param value the new value of the property.
 */
void update_property(entt::registry& registry,
                     comp::attribute_context& context,
                     std::string_view name,
                     attribute_value value);

/**
 * \brief Changes the type of the value of an existing property.
 *
 * \details The property will assume the default value of the specified type.
 *
 * \pre The context must feature a property with the specified name.
 *
 * \param registry the document registry.
 * \param context the context that will be modified.
 * \param name the name of the property that will be updated.
 * \param value the new type of the property.
 */
void change_property_type(entt::registry& registry,
                          comp::attribute_context& context,
                          std::string_view name,
                          attribute_type type);

/**
 * \brief Attempts to find a property in a context with a specific name.
 *
 * \param registry the document registry.
 * \param context the attribute context that will be queried.
 * \param name the name of the desired property.
 *
 * \return the found property entity; the null entity is returned if there was no match.
 *
 * \see has_property_with_name()
 * \see get_property()
 */
[[nodiscard]] auto find_property(const entt::registry& registry,
                                 const comp::attribute_context& context,
                                 std::string_view name) -> entt::entity;

/**
 * \brief Returns a property in an attribute context with a specific name.
 *
 * \param registry the document registry.
 * \param context the attribute context that will be queried.
 * \param name the name of the desired property.
 *
 * \return a reference to the matching property.
 *
 * \throws tactile_error if there is no property with the specified name.
 *
 * \see find_property()
 */
[[nodiscard]] auto get_property(const entt::registry& registry,
                                const comp::attribute_context& context,
                                std::string_view name) -> const comp::property&;

/**
 * \brief Indicates whether or not an attribute context has a property with a certain
 * name.
 *
 * \note Use the `find_property()` function if you intend to make use of the property
 * entity, in order to avoid unnecessary lookups.
 *
 * \param registry the document registry.
 * \param context the attribute context that will be queried..
 * \param name the name of the property to look for.
 *
 * \return `true` if the context has a property with the specified name; `false`
 * otherwise.
 *
 * \see find_property()
 */
[[nodiscard]] auto has_property_with_name(const entt::registry& registry,
                                          const comp::attribute_context& context,
                                          std::string_view name) -> bool;

/// \} End of property system

/// \} End of group property-system

}  // namespace tactile::sys
