// Copyright (C) 2024 Albin Johansson (GNU General Public License v3.0)

#pragma once

#include "tactile/base/container/function.hpp"
#include "tactile/base/container/hash_map.hpp"
#include "tactile/base/container/maybe.hpp"
#include "tactile/base/container/path.hpp"
#include "tactile/base/container/set.hpp"
#include "tactile/base/container/smart_ptr.hpp"
#include "tactile/base/prelude.hpp"
#include "tactile/core/util/uuid.hpp"

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
