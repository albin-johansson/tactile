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

#include "core/components/attributes.hpp"
#include "snapshot.hpp"
#include "tactile.hpp"

namespace tactile::sys {

/**
 * \ingroup systems
 * \defgroup context-system Context System
 */

/// \addtogroup context-system
/// \{

/**
 * \brief Resets the next context ID.
 *
 * \warning This function is provided for testing purposes, do not use otherwise.
 */
void reset_next_context_id() noexcept;

/**
 * \brief Increments the next available context ID and returns the previous value.
 *
 * \return the previous next context ID.
 */
[[nodiscard]] auto get_and_update_next_context_id() noexcept -> ContextID;

/**
 * \brief Returns the next available context ID.
 *
 * \return a valid context ID.
 */
[[nodiscard]] auto next_context_id() noexcept -> ContextID;

/**
 * \brief Adds an attribute context component to an entity.
 *
 * \details The created attribute context will automatically be assigned a unique ID.
 *
 * \pre The entity identifier cannot be null.
 *
 * \param registry the document registry.
 * \param entity the entity that will be assigned an attribute context.
 *
 * \return the created attribute context.
 */
auto add_attribute_context(entt::registry& registry, entt::entity entity)
    -> comp::AttributeContext&;

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
[[nodiscard]] auto get_context(entt::registry& registry, ContextID id)
    -> comp::AttributeContext&;

[[nodiscard]] auto get_context(const entt::registry& registry, ContextID id)
    -> const comp::AttributeContext&;

/**
 * \brief Returns the currently active attribute context.
 *
 * \param registry the document registry.
 *
 * \return the current context.
 */
[[nodiscard]] auto current_context(const entt::registry& registry)
    -> const comp::AttributeContext&;

/**
 * \brief Returns the identifier associated with the current context.
 *
 * \param registry the document registry.
 *
 * \return the ID of the current context.
 */
[[nodiscard]] auto current_context_id(const entt::registry& registry) -> ContextID;

/// \} End of group context-system

}  // namespace tactile::sys
