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

#include "core/commands/command.hpp"
#include "core/commands/command_id.hpp"
#include "core/common/math.hpp"
#include "core/common/memory.hpp"
#include "core/common/uuid.hpp"
#include "core/components/texture.hpp"
#include "core/fwd.hpp"

namespace tactile {

/// Command for creating a tileset and attaching it to a map document.
class AddTilesetCmd final : public ACommand
{
 public:
  /**
   * Creates a command.
   *
   * \param model the associated document model.
   * \param mapId the ID of the target map.
   * \param tilesetId the ID of the new tileset.
   * \param texture the tileset texture.
   * \param tileSize the tileset logical tile size.
   */
  AddTilesetCmd(DocumentModel* model,
                const UUID& mapId,
                const UUID& tilesetId,
                comp::Texture texture,
                const Vector2i& tileSize);

  void undo() override;

  void redo() override;

  [[nodiscard]] auto id() const noexcept -> int override
  {
    return CommandId::add_tileset;
  }

 private:
  DocumentModel* mModel{};
  UUID mMapId{};
  UUID mTilesetId{};
  comp::Texture mTexture;
  Vector2i mTileSize{};
  Shared<TilesetDocument> mTileset;  ///< The created tileset (initially null)
};

}  // namespace tactile
