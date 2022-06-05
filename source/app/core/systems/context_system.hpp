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

#include <entt/fwd.hpp>

#include "core/common/identifiers.hpp"
#include "core/common/uuid.hpp"
#include "core/components/attributes.hpp"
#include "snapshot.hpp"

namespace tactile::sys {

/**
 * \ingroup systems
 * \defgroup context-system Context System
 */

/// \addtogroup context-system
/// \{

/**
 * Destroys an entity and unregisters the associated context, if there is one.
 *
 * This function should always be used to destroy entities, even for non-context
 * entities, to maintain forward-compatibility.
 *
 * \param registry the associated registry.
 * \param entity the entity that will be destroyed.
 */
void destroy_entity(entt::registry& registry, entt::entity entity);

/**
 * Registers an entity as an attribute context.
 *
 * The created attribute context will automatically be assigned a unique ID. All attribute
 * contexts must be initialized using this function!
 *
 * \param registry the associated registry.
 * \param entity the entity that will be registered.
 *
 * \return the attached attribute context component.
 */
auto register_context(entt::registry& registry, entt::entity entity)
    -> comp::Context&;

/**
 * \brief Creates a snapshot of the state of a attribute context.
 *
 * \pre The supplied entity cannot be null.
 * \pre The supplied entity must feature a attribute context component.
 *
 * \param registry the source registry.
 * \param source the source entity that features a attribute context.
 *
 * \return a snapshot of the attribute context.
 *
 * \see RestorePropertyContext()
 */
[[nodiscard]] auto copy_attribute_context(const entt::registry& registry,
                                          entt::entity source)
    -> AttributeContextSnapshot;

/**
 * \brief Restores an attribute context component.
 *
 * \details This function will override any current attribute context component associated
 * with the specified entity.
 *
 * \pre The supplied entity identifier cannot be null.
 *
 * \param registry the document registry.
 * \param entity the entity that will be assigned the restored attribute context.
 * \param snapshot the state to restore.
 */
void restore_attribute_context(entt::registry& registry,
                               entt::entity entity,
                               AttributeContextSnapshot snapshot);

void set_context_id(entt::registry& registry,
                    comp::Context& context,
                    const UUID& id);

/**
 * Finds an entity with a specific context identifier.
 *
 * \param registry the source registry.
 * \param id the context identifier to look for.
 *
 * \return a possibly null entity.
 */
[[nodiscard]] auto find_context(const entt::registry& registry, const UUID& id)
    -> entt::entity;

/**
 * \brief Returns the context associated with an identifier.
 *
 * \param registry the document registry.
 * \param id the identifier associated with the desired context.
 *
 * \return a reference to the found context.
 *
 * \throws TactileError if the identifier is invalid.
 */
[[nodiscard]] auto get_context(entt::registry& registry, const UUID& id)
    -> comp::Context&;

[[nodiscard]] auto get_context(const entt::registry& registry, const UUID& id)
    -> const comp::Context&;

/**
 * \brief Returns the currently active attribute context.
 *
 * \param registry the document registry.
 *
 * \return the current context.
 */
[[nodiscard]] auto current_context(const entt::registry& registry)
    -> const comp::Context&;

/// \} End of group context-system

}  // namespace tactile::sys
