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

#include <glm/vec2.hpp>

#include "core/common/identifiers.hpp"
#include "core/common/ints.hpp"
#include "core/common/maybe.hpp"
#include "core/common/memory.hpp"
#include "core/common/ref.hpp"
#include "core/common/uuid.hpp"
#include "core/components/texture.hpp"
#include "core/systems/snapshot.hpp"
#include "editor/commands/command.hpp"
#include "editor/commands/command_id.hpp"
#include "editor/fwd.hpp"

namespace tactile {

/**
 * \brief Command for creating tilesets.
 */
class AddTilesetCmd final : public ACommand
{
 public:
  AddTilesetCmd(DocumentModel* model,
                const UUID& mapId,
                const UUID& tilesetId,
                comp::Texture texture,
                const glm::ivec2& tileSize);

  void undo() override;

  void redo() override;

  [[nodiscard]] auto id() const noexcept -> int override
  {
    return CommandId::add_tileset;
  }

 private:
  DocumentModel* mModel{};
  UUID mMapId{};      ///< The ID of the map document the tileset should be added to.
  UUID mTilesetId{};  ///< The ID of the added tileset document.

  comp::Texture mTexture;
  glm::ivec2 mTileSize{};

  Shared<TilesetDocument> mTileset;
  Maybe<sys::TilesetSnapshot> mSnapshot;
};

}  // namespace tactile
