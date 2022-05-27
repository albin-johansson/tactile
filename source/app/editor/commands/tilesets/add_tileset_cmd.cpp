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

#include "add_tileset_cmd.hpp"

#include <memory>   // make_shared
#include <utility>  // move

#include "core/systems/tileset_system.hpp"
#include "editor/documents/map_document.hpp"
#include "editor/documents/tileset_document.hpp"
#include "editor/model.hpp"
#include "misc/panic.hpp"

namespace tactile {

AddTilesetCmd::AddTilesetCmd(DocumentModel* model,
                             const UUID& mapId,
                             const UUID& tilesetId,
                             comp::Texture texture,
                             const Vector2i& tileSize)
    : ACommand{"Add Tileset"}
    , mModel{model}
    , mMapId{mapId}
    , mTilesetId{tilesetId}
    , mTexture{std::move(texture)}
    , mTileSize{tileSize}
{
  if (!mModel) {
    throw TactileError{"Invalid null model!"};
  }
}

void AddTilesetCmd::undo()
{
  auto map = mModel->get_map(mMapId);
  auto& mapRegistry = map->get_registry();

  // TODO deal with tileset document potentially being open

  sys::detach_tileset(mapRegistry, mTilesetId);
}

void AddTilesetCmd::redo()
{
  if (!mTileset) {
    mTileset = std::make_shared<TilesetDocument>(mTilesetId, mTexture, mTileSize);
  }
  mModel->register_tileset(mTileset);

  auto map = mModel->get_map(mMapId);
  auto& mapRegistry = map->get_registry();

  sys::attach_tileset(mapRegistry, mTilesetId, mTileset->info());
}

}  // namespace tactile
