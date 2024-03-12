// Copyright (C) 2024 Albin Johansson (GNU General Public License v3.0)

#pragma once

#include "core/tile/tile_extent.hpp"
#include "model/document/document_manager.hpp"
#include "model/model.hpp"
#include "tactile/base/container/path.hpp"
#include "tactile/base/container/smart_ptr.hpp"
#include "tactile/base/int.hpp"
#include "tactile/base/prelude.hpp"
#include "tactile/core/numeric/vec.hpp"
#include "tactile/core/util/uuid.hpp"

namespace tactile {

TACTILE_FWD_DECLARE_CLASS(Document)
TACTILE_FWD_DECLARE_CLASS(MapDocument)
TACTILE_FWD_DECLARE_CLASS(TilesetDocument)
TACTILE_FWD_DECLARE_STRUCT(TilesetInfo)

/// Implementation used by the DocumentModel class, not to be used directly elsewhere.
class DocumentModel::Impl {
 public:
  explicit Impl(DocumentModel* parent);

  void update();

  void each(const VisitorFunc& func) const;

  auto add_map(const Int2& tile_size, TileExtent extent) -> UUID;

  auto add_tileset(const TilesetInfo& info) -> UUID;

  void remove_tileset(const UUID& id);

  auto restore_tileset(TileID first_tile_id, const TilesetInfo& info) -> UUID;

  void set_command_capacity(usize capacity);

  [[nodiscard]] auto has_document(const UUID& id) const -> bool;

  [[nodiscard]] auto has_document_with_path(const Path& path) const -> bool;

  [[nodiscard]] auto get_id_for_path(const Path& path) const -> UUID;

  [[nodiscard]] auto require_active_document() const -> const Document&;

  [[nodiscard]] auto require_active_map() -> MapDocument&;
  [[nodiscard]] auto require_active_map() const -> const MapDocument&;

  [[nodiscard]] auto require_active_tileset() -> TilesetDocument&;
  [[nodiscard]] auto require_active_tileset() const -> const TilesetDocument&;

  [[nodiscard]] auto documents() -> DocumentManager&;

 private:
  DocumentModel* mParent {};
  DocumentManager mDocuments;
};

}  // namespace tactile
