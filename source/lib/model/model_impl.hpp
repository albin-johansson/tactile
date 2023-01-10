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

#include "common/macros.hpp"
#include "common/numeric.hpp"
#include "common/type/math.hpp"
#include "common/type/path.hpp"
#include "common/type/ptr.hpp"
#include "common/type/uuid.hpp"
#include "model/document/document_manager.hpp"
#include "model/model.hpp"

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

  auto add_map(const Int2& tile_size, usize rows, usize columns) -> UUID;

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
