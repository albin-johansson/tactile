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

#include "common/type/func.hpp"
#include "common/type/math.hpp"
#include "common/type/maybe.hpp"
#include "common/type/path.hpp"
#include "common/type/ptr.hpp"
#include "common/type/uuid.hpp"
#include "core/tile/tile_extent.hpp"
#include "model/document/document.hpp"
#include "model/tool/tool_type.hpp"
#include "tactile/base/id.hpp"
#include "tactile/base/int.hpp"
#include "tactile/base/prelude.hpp"

namespace tactile {

TACTILE_FWD_DECLARE_STRUCT(TilesetInfo)
TACTILE_FWD_DECLARE_CLASS(MapDocument)
TACTILE_FWD_DECLARE_CLASS(TilesetDocument)

TACTILE_FWD_DECLARE_CLASS_NS(cmd, CreateTileset)
TACTILE_FWD_DECLARE_CLASS_NS(cmd, RemoveTileset)

/// The primary facade between the editor and the underlying core documents.
class DocumentModel final {
  friend class cmd::CreateTileset;
  friend class cmd::RemoveTileset;

 public:
  using VisitorFunc = Func<void(const UUID&)>;

  DocumentModel();
  ~DocumentModel() noexcept;

  /// Updates systems that need to be consistently updated, e.g. animations.
  void update();

  /// Visits each open document.
  void each(const VisitorFunc& func) const;

  /// Creates an empty map document with the specified attributes.
  auto create_map_document(const Int2& tile_size, TileExtent extent) -> UUID;

  /// Creates a tileset document and adds it to the active map document.
  auto add_tileset(const TilesetInfo& info) -> UUID;

  /// Restores a tileset, intended to be used after parsing maps.
  auto restore_tileset(TileID first_tile_id, const TilesetInfo& info) -> UUID;

  /// Makes a specific document active.
  void select_document(const UUID& id);

  /// Opens a document in a separate tab.
  void open_document(const UUID& id);

  /// Closes an open document, only removing it if it's a map document.
  void close_document(const UUID& id);

  /// Removes a tileset from the active map document.
  void remove_tileset(const UUID& id);

  /// Sets the undo stack capacity of all loaded documents.
  void set_command_capacity(usize capacity);

  [[nodiscard]] auto has_document(const UUID& id) const -> bool;

  /// Indicates whether any of the loaded documents are located at the specific path.
  [[nodiscard]] auto has_document_with_path(const Path& path) const -> bool;

  /// Returns the ID of the loaded document with the specified path.
  [[nodiscard]] auto get_id_for_path(const Path& path) const -> UUID;

  /// Indicates whether there is an active document, of any type.
  [[nodiscard]] auto has_active_document() const -> bool;

  /// Indicates whether a map document is currently active.
  [[nodiscard]] auto is_map_active() const -> bool;

  /// Indicates whether a tileset document is currently active.
  [[nodiscard]] auto is_tileset_active() const -> bool;

  /// Returns a pointer to the active document, if there is one.
  [[nodiscard]] auto active_document() -> Document*;
  [[nodiscard]] auto active_document() const -> const Document*;

  [[nodiscard]] auto active_map_document() -> MapDocument*;
  [[nodiscard]] auto active_map_document() const -> const MapDocument*;

  [[nodiscard]] auto active_tileset_document() -> TilesetDocument*;
  [[nodiscard]] auto active_tileset_document() const -> const TilesetDocument*;

  [[nodiscard]] auto require_active_document() const -> const Document&;

  [[nodiscard]] auto require_active_map_document() -> MapDocument&;
  [[nodiscard]] auto require_active_map_document() const -> const MapDocument&;

  [[nodiscard]] auto require_active_tileset_document() -> TilesetDocument&;
  [[nodiscard]] auto require_active_tileset_document() const -> const TilesetDocument&;

  /// Indicates whether a document is open.
  [[nodiscard]] auto is_open(const UUID& id) const -> bool;

  /// Indicates whether a document represents a map.
  [[nodiscard]] auto is_map(const UUID& id) const -> bool;

  /// Indicates whether a document represents a tileset.
  [[nodiscard]] auto is_tileset(const UUID& id) const -> bool;

  [[nodiscard]] auto get_document_ptr(const UUID& id) -> Shared<Document>;
  [[nodiscard]] auto get_map_document_ptr(const UUID& id) -> Shared<MapDocument>;
  [[nodiscard]] auto get_tileset_document_ptr(const UUID& id) -> Shared<TilesetDocument>;

  /// Getters that do not require a copy of shared pointers, which can be expensive
  [[nodiscard]] auto get_document(const UUID& id) const -> const Document&;
  [[nodiscard]] auto get_map_document(const UUID& id) const -> const MapDocument&;
  [[nodiscard]] auto get_tileset_document(const UUID& id) const -> const TilesetDocument&;

  [[nodiscard]] auto get_active_document_id() const -> Maybe<UUID>;

 private:
  class Impl;
  Unique<Impl> mImpl;

  void register_map(Shared<MapDocument> document);
  void register_tileset(Shared<TilesetDocument> document);

  auto unregister_map(const UUID& id) -> Shared<MapDocument>;
  auto unregister_tileset(const UUID& id) -> Shared<TilesetDocument>;
};

}  // namespace tactile
