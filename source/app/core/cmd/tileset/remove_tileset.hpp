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

#include "core/cmd/command.hpp"
#include "core/type/maybe.hpp"
#include "core/type/ptr.hpp"
#include "core/uuid.hpp"
#include "core/vocabulary.hpp"

namespace tactile {
class DocumentModel;
class TilesetDocument;
}  // namespace tactile

namespace tactile::cmd {

/// Command for removing a tileset from the active map document.
class RemoveTileset final : public Command {
 public:
  RemoveTileset(DocumentModel* model, const UUID& tileset_id);

  void undo() override;

  void redo() override;

  [[nodiscard]] auto get_name() const -> String override;

 private:
  DocumentModel* mModel {};
  UUID mTilesetId {};
  UUID mMapId {};
  Shared<TilesetDocument> mTilesetDocument;
  Maybe<TileID> mFirstTile;
};

}  // namespace tactile::cmd
