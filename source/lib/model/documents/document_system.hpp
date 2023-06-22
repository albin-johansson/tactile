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

#include <utility>  // forward

#include "cmd/command_stack.hpp"
#include "common/tile_extent.hpp"
#include "common/type/ecs.hpp"
#include "common/type/math.hpp"
#include "common/type/path.hpp"
#include "model/components/map.hpp"
#include "model/event/map_events.hpp"
#include "model/model.hpp"

namespace tactile::sys {

/**
 * Disposes an existing document.
 *
 * \details The document is automatically closed by this function. Additionally, the
 * active document property is reset if the document is active.
 *
 * \param model           the associated model.
 * \param document_entity a valid document entity.
 */
void destroy_document(Model& model, Entity document_entity);

/**
 * Selects an existing document, making it "active".
 *
 * \pre The specified document must be open.
 *
 * \param model           the associated model.
 * \param document_entity a valid document entity.
 */
void select_document(Model& model, Entity document_entity);

/**
 * Opens an existing document, making it show up as tab in the primary viewport widget.
 *
 * \details This function has no effect if the document is already open.
 *
 * \param model           the associated model.
 * \param document_entity a valid document entity.
 */
void open_document(Model& model, Entity document_entity);

/**
 * Closes an existing document.
 *
 * \todo Determine behavior when closing the active document.
 *
 * \pre The specified document must be open.
 *
 * \param model           the associated model.
 * \param document_entity a valid document entity.
 */
void close_document(Model& model, Entity document_entity);

/**
 * Sets the name of a document.
 *
 * \param model           the associated model.
 * \param document_entity a valid document entity.
 * \param name            the new document name.
 */
void set_document_name(Model& model, Entity document_entity, String name);

/**
 * Returns the name of a document.
 *
 * \param model           the associated model.
 * \param document_entity a valid document entity.
 *
 * \return the document name.
 */
[[nodiscard]] auto get_document_name(const Model& model, Entity document_entity)
    -> String;

/**
 * Returns the currently active document (if there is one).
 *
 * \param model the associated model.
 *
 * \return a document entity or null.
 */
[[nodiscard]] auto get_active_document(const Model& model) -> Entity;

/**
 * Returns the active context in the currently active document.
 *
 * \param model the associated model.
 *
 * \return a context entity or null.
 */
[[nodiscard]] auto get_active_context(const Model& model) -> Entity;

/**
 * Returns the currently active map (if there is one).
 *
 * \param model the associated model.
 *
 * \return a map entity or null.
 */
[[nodiscard]] auto get_active_map(const Model& model) -> Entity;

[[nodiscard]] auto try_get_active_map(const Model& model) -> const Map*;

/**
 * Returns the tileset document associated with a tileset.
 *
 * \param model          the associated model.
 * \param tileset_entity a valid tileset entity.
 *
 * \return a tileset document entity, or the null entity.
 */
[[nodiscard]] auto get_associated_tileset_document(const Model& model,
                                                   Entity tileset_entity) -> Entity;

/**
 * Indicates whether any loaded document is associated with the specified path.
 *
 * \param model the associated model.
 * \param path  the file path to look for.
 *
 * \return true if a document was found; false otherwise.
 */
[[nodiscard]] auto has_document_with_path(const Model& model, const Path& path) -> bool;

[[nodiscard]] auto get_document_with_path(const Model& model, const Path& path) -> Entity;

/// Indicates whether any document is active.
[[nodiscard]] auto has_active_document(const Model& model) -> bool;

/// Indicates whether any map document is active.
[[nodiscard]] auto is_map_document_active(const Model& model) -> bool;

/// Indicates whether any tileset document is active.
[[nodiscard]] auto is_tileset_document_active(const Model& model) -> bool;

/**
 * Indicates whether a specific document is open.
 *
 * \param model           the associated model.
 * \param document_entity a valid document entity.
 *
 * \return true if the document is open; false otherwise.
 */
[[nodiscard]] auto is_document_open(const Model& model, Entity document_entity) -> bool;

/**
 * Creates a map document, opens it, and makes it the active document.
 *
 * \param model the associated model.
 * \param event information about the map that will be created.
 */
void on_create_map(Model& model, const CreateMapEvent& event);

}  // namespace tactile::sys
