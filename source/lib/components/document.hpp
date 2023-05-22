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

#include "common/type/ecs.hpp"
#include "common/type/maybe.hpp"
#include "common/type/path.hpp"
#include "common/type/set.hpp"

namespace tactile {

enum class DocumentType {
  Unknown,
  Map,
  Tileset,
};

/// Context component that tracks the active and open documents.
struct DocumentContext final {
  Entity active_document {kNullEntity};  ///< The currently active document, if any.
  Set<Entity> open_documents;            ///< The currently open documents.
};

/// General component for editor documents.
/// \FIXME the active context member is error-prone as it stands (might be null)
struct Document final {
  DocumentType type {DocumentType::Unknown};  ///< The specific document type.
  Entity component_set {kNullEntity};         ///< The associated component set.
  Entity active_context {kNullEntity};        ///< The currently active context.
  Entity default_context {kNullEntity};       ///< Context entity used as a fallback.
  Maybe<Path> path;                           ///< File path to the document.

  [[nodiscard]] auto get_active_context() const -> Entity
  {
    return (active_context != kNullEntity) ? active_context : default_context;
  }
};

/// Component featured by map document entities.
struct MapDocument final {
  Entity map {kNullEntity};             ///< The associated map.
  Entity active_tileset {kNullEntity};  ///< The active attached tileset, if any.
};

/// Component featured by tileset document entities.
struct TilesetDocument final {
  Entity tileset {kNullEntity};  ///< The associated tileset.
};

}  // namespace tactile
