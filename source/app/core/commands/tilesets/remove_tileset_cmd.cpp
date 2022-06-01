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

#include "remove_tileset_cmd.hpp"

#include "core/documents/map_document.hpp"
#include "core/documents/tileset_document.hpp"
#include "core/model.hpp"
#include "core/systems/tilesets/tileset_system.hpp"
#include "misc/assert.hpp"
#include "misc/panic.hpp"

namespace tactile {

RemoveTilesetCmd::RemoveTilesetCmd(DocumentModel* model, const UUID& tilesetId)
    : ACommand{"Remove Tileset"}
    , mModel{model}
    , mTilesetId{tilesetId}
{
  if (!mModel) {
    throw TactileError{"Invalid null model!"};
  }

  TACTILE_ASSERT(mModel->active_document_id().has_value());
  mMapId = mModel->active_document_id().value();
}

void RemoveTilesetCmd::undo()
{
  TACTILE_ASSERT(mTileset != nullptr);
  TACTILE_ASSERT(mModel->active_document_id() == mMapId);

  auto map = mModel->get_map(mMapId);
  auto& mapRegistry = map->get_registry();

  sys::attach_tileset(mapRegistry, mTilesetId, mTileset->info(), mFirstTile.value());

  // TODO select tileset
}

void RemoveTilesetCmd::redo()
{
  TACTILE_ASSERT(mModel->active_document_id() == mMapId);

  mTileset = mModel->get_tileset(mTilesetId);

  // TODO close the document, if open

  auto map = mModel->get_map(mMapId);
  auto& mapRegistry = map->get_registry();

  const auto tilesetEntity = sys::find_tileset(mapRegistry, mTilesetId);
  const auto& ref = checked_get<comp::TilesetRef>(mapRegistry, tilesetEntity);
  mFirstTile = ref.first_id;

  auto& activeTileset = ctx_get<comp::ActiveTileset>(mapRegistry);

  if (tilesetEntity == activeTileset.entity) {
    activeTileset.entity = entt::null;  // TODO try to pick another tileset
  }

  // TODO use the snapshot? (maybe not necessary if tset document is isolated)
  sys::detach_tileset(mapRegistry, mTilesetId);
}

}  // namespace tactile
