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

#include "core/type/hash_map.hpp"
#include "core/type/math.hpp"
#include "core/type/maybe.hpp"
#include "core/type/path.hpp"
#include "core/type/ptr.hpp"
#include "core/type/vector.hpp"
#include "core/uuid.hpp"
#include "core/vocabulary.hpp"
#include "model/model.hpp"

namespace tactile {

class Document;
class MapDocument;
class TilesetDocument;
struct TilesetInfo;

/// Implementation used by the DocumentModel class, not to be used directly elsewhere.
class DocumentModel::Impl {
 public:
  explicit Impl(DocumentModel* parent);

  void update();

  void each(const VisitorFunc& func) const;

  auto add_map(const Int2& tile_size, usize rows, usize columns) -> UUID;

  auto add_tileset(const TilesetInfo& info) -> UUID;

  void remove_tileset(const UUID& id);

  auto restore_tileset(TileID first_tile_id, const TilesetInfo& info) -> UUID;

  void select_document(const UUID& id);
  void open_document(const UUID& id);
  void close_document(const UUID& id);

  void set_command_capacity(usize capacity);

  [[nodiscard]] auto has_document(const UUID& id) const -> bool;

  [[nodiscard]] auto has_document_with_path(const Path& path) const -> bool;

  [[nodiscard]] auto get_id_for_path(const Path& path) const -> UUID;

  [[nodiscard]] auto has_active_document() const -> bool;

  [[nodiscard]] auto is_map_active() const -> bool;
  [[nodiscard]] auto is_tileset_active() const -> bool;

  [[nodiscard]] auto active_document() -> Document*;
  [[nodiscard]] auto active_document() const -> const Document*;

  [[nodiscard]] auto active_map() -> MapDocument*;
  [[nodiscard]] auto active_map() const -> const MapDocument*;

  [[nodiscard]] auto active_tileset() -> TilesetDocument*;
  [[nodiscard]] auto active_tileset() const -> const TilesetDocument*;

  [[nodiscard]] auto require_active_document() const -> const Document&;

  [[nodiscard]] auto require_active_map() -> MapDocument&;
  [[nodiscard]] auto require_active_map() const -> const MapDocument&;

  [[nodiscard]] auto require_active_tileset() -> TilesetDocument&;
  [[nodiscard]] auto require_active_tileset() const -> const TilesetDocument&;

  [[nodiscard]] auto is_open(const UUID& id) const -> bool;

  [[nodiscard]] auto is_map(const UUID& id) const -> bool;
  [[nodiscard]] auto is_tileset(const UUID& id) const -> bool;

  [[nodiscard]] auto get_document(const UUID& id) -> Shared<Document>;
  [[nodiscard]] auto get_map(const UUID& id) -> Shared<MapDocument>;
  [[nodiscard]] auto get_tileset(const UUID& id) -> Shared<TilesetDocument>;

  [[nodiscard]] auto view_document(const UUID& id) const -> const Document&;
  [[nodiscard]] auto view_map(const UUID& id) const -> const MapDocument&;
  [[nodiscard]] auto view_tileset(const UUID& id) const -> const TilesetDocument&;

  [[nodiscard]] auto active_document_id() const -> Maybe<UUID>;

  void select_another_document();

  void remove_unreferenced_tilesets(const MapDocument& doc);

  void register_map(Shared<MapDocument> document);
  void register_tileset(Shared<TilesetDocument> document);

  auto unregister_map(const UUID& id) -> Shared<MapDocument>;
  auto unregister_tileset(const UUID& id) -> Shared<TilesetDocument>;

  [[nodiscard]] auto is_tileset_referenced(const UUID& tileset_id) const -> bool;

 private:
  DocumentModel* mParent {};
  HashMap<UUID, Shared<Document>> mDocuments;        /// All loaded documents.
  HashMap<UUID, Shared<MapDocument>> mMaps;          /// All maps.
  HashMap<UUID, Shared<TilesetDocument>> mTilesets;  /// All tilesets.
  Vec<UUID> mOpenDocuments;     /// Documents that are open in the editor.
  Maybe<UUID> mActiveDocument;  /// The active document ID.
};

}  // namespace tactile
