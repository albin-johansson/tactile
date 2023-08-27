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

#include "common/type/uuid.hpp"
#include "tactile/core/io/filesystem.hpp"
#include "tactile/core/prelude.hpp"
#include "tactile/core/type/function.hpp"
#include "tactile/core/type/hash_map.hpp"
#include "tactile/core/type/maybe.hpp"
#include "tactile/core/type/set.hpp"
#include "tactile/core/type/smart_ptr.hpp"

namespace tactile {

TACTILE_FWD_DECLARE_CLASS(Document)
TACTILE_FWD_DECLARE_CLASS(MapDocument)
TACTILE_FWD_DECLARE_CLASS(TilesetDocument)
TACTILE_FWD_DECLARE_CLASS(TilesetBundle)

/// Responsible for the storage of documents.
class DocumentManager final {
 public:
  using DocVisitorFn = Function<void(const Shared<Document>&)>;
  using IdVisitorFn = Function<void(const UUID&)>;
  using PredicateFn = Function<bool(const Document&)>;

  void each(const DocVisitorFn& op) const;

  void each_open(const IdVisitorFn& op) const;

  void add_map_document(Shared<MapDocument> document);
  void add_tileset_document(Shared<TilesetDocument> document);

  auto remove_map_document(const UUID& id) -> Shared<MapDocument>;
  auto remove_tileset_document(const UUID& id) -> Shared<TilesetDocument>;

  void select_another_document();

  void select_document(const UUID& id);

  void open_document(const UUID& id);
  void close_document(const UUID& id);

  void remove_unused_tilesets_from(const TilesetBundle& bundle);

  [[nodiscard]] auto current_document_id() const -> const Maybe<UUID>&;

  [[nodiscard]] auto current_document() -> Document*;
  [[nodiscard]] auto current_document() const -> const Document*;

  [[nodiscard]] auto current_map_document() -> MapDocument*;
  [[nodiscard]] auto current_map_document() const -> const MapDocument*;

  [[nodiscard]] auto current_tileset_document() -> TilesetDocument*;
  [[nodiscard]] auto current_tileset_document() const -> const TilesetDocument*;

  [[nodiscard]] auto is_map_active() const -> bool;
  [[nodiscard]] auto is_tileset_active() const -> bool;

  [[nodiscard]] auto is_document_open(const UUID& id) const -> bool;

  [[nodiscard]] auto is_document(const UUID& id) const -> bool;
  [[nodiscard]] auto is_map(const UUID& id) const -> bool;
  [[nodiscard]] auto is_tileset(const UUID& id) const -> bool;

  [[nodiscard]] auto is_tileset_used(const UUID& id) const -> bool;

  [[nodiscard]] auto has_document_with_path(const Path& path) const -> bool;

  [[nodiscard]] auto get_document_ptr(const UUID& id) -> Shared<Document>;
  [[nodiscard]] auto get_map_document_ptr(const UUID& id) -> Shared<MapDocument>;
  [[nodiscard]] auto get_tileset_document_ptr(const UUID& id) -> Shared<TilesetDocument>;

  [[nodiscard]] auto get_document(const UUID& id) const -> const Document&;
  [[nodiscard]] auto get_map_document(const UUID& id) const -> const MapDocument&;
  [[nodiscard]] auto get_tileset_document(const UUID& id) const -> const TilesetDocument&;

  [[nodiscard]] auto first_match(const PredicateFn& pred) const -> Maybe<UUID>;

 private:
  HashMap<UUID, Shared<Document>> mDocuments;        /// All loaded documents.
  HashMap<UUID, Shared<MapDocument>> mMaps;          /// All maps.
  HashMap<UUID, Shared<TilesetDocument>> mTilesets;  /// All tilesets.
  Maybe<UUID> mActiveDocument;                       /// The active document ID.
  Set<UUID> mOpenDocuments;                          /// Documents open in the editor.
};

}  // namespace tactile
