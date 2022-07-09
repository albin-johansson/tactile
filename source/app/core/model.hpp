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

#include <filesystem>  // path
#include <functional>  // function
#include <vector>      // vector

#include <boost/uuid/uuid_hash.hpp>

#include "core/common/associative.hpp"
#include "core/common/identifiers.hpp"
#include "core/common/ints.hpp"
#include "core/common/math.hpp"
#include "core/common/maybe.hpp"
#include "core/common/memory.hpp"
#include "core/common/uuid.hpp"
#include "core/documents/document.hpp"
#include "core/fwd.hpp"
#include "core/tools/tool_type.hpp"

namespace tactile {

/// The primary facade between the editor and the underlying core documents.
class DocumentModel final
{
  friend class AddTilesetCmd;
  friend class RemoveTilesetCmd;

 public:
  using VisitorFunc = std::function<void(const UUID&)>;

  /// Updates systems that need to be consistently updated, e.g. animations.
  void update();

  /// Visits each open document.
  void each(const VisitorFunc& func) const;

  /**
   * Creates an empty map document with the specified attributes.
   *
   * \param tileSize the global tile size in the map.
   * \param rows the initial amount of rows in the map.
   * \param columns the initial amount of columns in the map.
   *
   * \return the map document ID.
   */
  auto add_map(const Vector2i& tileSize, usize rows, usize columns) -> UUID;

  /// Creates a tileset document and adds it to the active map document.
  auto add_tileset(const TilesetInfo& info) -> UUID;

  /// Restores a tileset, intended to be used after parsing maps.
  auto restore_tileset(TileID firstTileId, const TilesetInfo& info) -> UUID;

  /// Makes a specific document active.
  void select_document(const UUID& id);

  /// Opens a document as a separate tab.
  void open_document(const UUID& id);

  /// Closes an open document, but does not remove it!
  void close_document(const UUID& id);

  /// Removes a tileset from the active map document.
  void remove_tileset(const UUID& id);

  /// Sets the undo stack capacity of all loaded documents.
  void set_command_capacity(usize capacity);

  /// Indicates whether any of the loaded documents are located at the specific path.
  [[nodiscard]] auto has_document_with_path(const std::filesystem::path& path) const
      -> bool;

  /// Returns the document ID of the loaded document with the specified path.
  [[nodiscard]] auto get_id_for_path(const std::filesystem::path& path) const -> UUID;

  /// Indicates whether there is an active document, of any type.
  [[nodiscard]] auto has_active_document() const -> bool;

  /// Indicates whether a map document is currently active.
  [[nodiscard]] auto is_map_active() const -> bool;

  /// Indicates whether a tileset document is currently active.
  [[nodiscard]] auto is_tileset_active() const -> bool;

  /// Returns a pointer to the active document, if there is one.
  [[nodiscard]] auto active_document() -> ADocument*;
  [[nodiscard]] auto active_document() const -> const ADocument*;

  [[nodiscard]] auto active_map() -> MapDocument*;
  [[nodiscard]] auto active_map() const -> const MapDocument*;

  [[nodiscard]] auto active_tileset() -> TilesetDocument*;
  [[nodiscard]] auto active_tileset() const -> const TilesetDocument*;

  [[nodiscard]] auto require_active_document() const -> const ADocument&;

  [[nodiscard]] auto require_active_map() -> MapDocument&;
  [[nodiscard]] auto require_active_map() const -> const MapDocument&;

  [[nodiscard]] auto require_active_tileset() -> TilesetDocument&;
  [[nodiscard]] auto require_active_tileset() const -> const TilesetDocument&;

  /// Indicates whether a document is open.
  [[nodiscard]] auto is_open(const UUID& id) const -> bool;

  /// Indicates whether a document represents a map.
  [[nodiscard]] auto is_map(const UUID& id) const -> bool;

  /// Indicates whether a document represents a tileset.
  [[nodiscard]] auto is_tileset(const UUID& id) const -> bool;

  [[nodiscard]] auto get_document(const UUID& id) -> Shared<ADocument>;
  [[nodiscard]] auto get_map(const UUID& id) -> Shared<MapDocument>;
  [[nodiscard]] auto get_tileset(const UUID& id) -> Shared<TilesetDocument>;

  /// Getters that do not require a copy of shared pointers, which can be expensive
  [[nodiscard]] auto view_document(const UUID& id) const -> const ADocument&;
  [[nodiscard]] auto view_map(const UUID& id) const -> const MapDocument&;
  [[nodiscard]] auto view_tileset(const UUID& id) const -> const TilesetDocument&;

  [[nodiscard]] auto active_document_id() const -> Maybe<UUID>;

 private:
  HashMap<UUID, Shared<ADocument>>       mDocuments;  /// All _loaded_ documents
  HashMap<UUID, Shared<MapDocument>>     mMaps;
  HashMap<UUID, Shared<TilesetDocument>> mTilesets;
  std::vector<UUID> mOpenDocuments;   /// All _open_ documents in the editor
  Maybe<UUID>       mActiveDocument;  /// ID of the active document.

  void register_map(Shared<MapDocument> document);
  void register_tileset(Shared<TilesetDocument> document);

  auto unregister_map(const UUID& id) -> Shared<MapDocument>;
  auto unregister_tileset(const UUID& id) -> Shared<TilesetDocument>;
};

}  // namespace tactile
