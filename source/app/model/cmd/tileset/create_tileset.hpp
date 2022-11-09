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

#include "core/tile/tileset_info.hpp"
#include "core/type/math.hpp"
#include "core/type/ptr.hpp"
#include "core/uuid.hpp"
#include "core/vocabulary.hpp"
#include "model/cmd/command.hpp"

TACTILE_FWD_DECLARE_CLASS_NS(tactile, DocumentModel)
TACTILE_FWD_DECLARE_CLASS_NS(tactile, TilesetDocument)

namespace tactile::cmd {

/// Command for creating a tileset and attaching it to a map document.
class CreateTileset final : public Command {
 public:
  CreateTileset(DocumentModel* model,
                const UUID& map_id,
                const UUID& tileset_id,
                TilesetInfo info);

  void undo() override;

  void redo() override;

  [[nodiscard]] auto get_name() const -> String override;

 private:
  DocumentModel* mModel {};
  UUID mMapId {};
  UUID mTilesetId {};
  TilesetInfo mTilesetInfo;
  Shared<TilesetDocument> mTileset;  /// The created tileset
};

}  // namespace tactile::cmd
