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

#include <entt/fwd.hpp>

#include "core/attribute.hpp"
#include "core/fwd.hpp"
#include "core/systems/snapshot.hpp"

namespace tactile::sys {

/**
 * Adds a property to an attribute context.
 *
 * \pre The supplied name must be unique within the attribute context.
 *
 * \param registry the document registry.
 * \param context the context that will be modified.
 * \param name the name of the property.
 * \param type the type of the property value.
 */
void add_property(entt::registry& registry,
                  comp::Context& context,
                  std::string name,
                  AttributeType type);

/**
 * Adds a property to an attribute context.
 *
 * \pre The name must be unique within the attribute context.
 *
 * \param registry the document registry.
 * \param context the context that will be modified.
 * \param name the name of the property.
 * \param value the initial value of the property.
 */
void add_property(entt::registry& registry,
                  comp::Context& context,
                  std::string name,
                  Attribute value);

/**
 * Removes a property from an attribute context.
 *
 * \pre The context must feature a property with the specified name.
 *
 * \param registry the document registry.
 * \param context the context that will be modified.
 * \param name the name of the property that will be removed.
 */
void remove_property(entt::registry& registry,
                     comp::Context& context,
                     std::string_view name);

/**
 * Renames a property in an attribute context.
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
                     comp::Context& context,
                     std::string_view oldName,
                     std::string newName);

/**
 * Updates the value of an existing property.
 *
 * \pre The context must feature a property with the specified name.
 *
 * \param registry the document registry.
 * \param context the context that will be modified.
 * \param name the name of the property that will be updated.
 * \param value the new value of the property.
 */
void update_property(entt::registry& registry,
                     comp::Context& context,
                     std::string_view name,
                     Attribute value);

/**
 * Changes the type of the value of an existing property.
 *
 * The property will assume the default value of the specified type.
 *
 * \pre The context must feature a property with the specified name.
 *
 * \param registry the document registry.
 * \param context the context that will be modified.
 * \param name the name of the property that will be updated.
 * \param type the new type of the property.
 */
void change_property_type(entt::registry& registry,
                          comp::Context& context,
                          std::string_view name,
                          AttributeType type);

/**
 * Attempts to find a property in a context with a specific name.
 *
 * \param registry the document registry.
 * \param context the attribute context that will be queried.
 * \param name the name of the desired property.
 *
 * \return the found property entity; the null entity is returned if there was no match.
 */
[[nodiscard]] auto find_property(const entt::registry& registry,
                                 const comp::Context& context,
                                 std::string_view name) -> entt::entity;

/**
 * Returns a property in an attribute context with a specific name.
 *
 * \param registry the document registry.
 * \param context the attribute context that will be queried.
 * \param name the name of the desired property.
 *
 * \return a reference to the matching property.
 *
 * \throws TactileError if there is no property with the specified name.
 */
[[nodiscard]] auto get_property(const entt::registry& registry,
                                const comp::Context& context,
                                std::string_view name) -> const comp::Property&;

/**
 * Indicates whether or not an attribute context has a property with a certain name.
 *
 * Note, use the `find_property()` function if you intend to make use of the property
 * entity, in order to avoid unnecessary lookups.
 *
 * \param registry the document registry.
 * \param context the attribute context that will be queried..
 * \param name the name of the property to look for.
 *
 * \return `true` if the context has a property with the specified name;
 *         `false` otherwise.
 */
[[nodiscard]] auto has_property_with_name(const entt::registry& registry,
                                          const comp::Context& context,
                                          std::string_view name) -> bool;

}  // namespace tactile::sys
