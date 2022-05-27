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

#include "core/systems/tileset_system.hpp"
#include "editor/model.hpp"
#include "misc/assert.hpp"
#include "misc/panic.hpp"

namespace tactile {

RemoveTilesetCmd::RemoveTilesetCmd(DocumentModel* model, const UUID& tilesetId)
    : ACommand{"Remove Tileset"}
    , mModel{model}
    , mTilesetId{tilesetId}
{
  if (!mModel) {
    throw TactileError("Invalid null model!");
  }

  TACTILE_ASSERT(mModel->active_document_id().has_value());
  mMapId = mModel->active_document_id().value();
}

void RemoveTilesetCmd::undo()
{
  //  auto& registry = mRegistry.get();
  //  sys::restore_tileset(registry, mSnapshot.value());
  //
  //  auto& active = registry.ctx().at<comp::ActiveTileset>();
  //  active.entity = sys::find_tileset(registry, mTilesetId);
}

void RemoveTilesetCmd::redo()
{
//  auto& document = mModel->get_document(mMapId);
//  sys::detach_tileset(document.registry, mTilesetId);

  //  const auto entity = sys::find_tileset(mRegistry, mTilesetId);
  //  TACTILE_ASSERT(entity != entt::null);
  //
  //  mSnapshot = sys::copy_tileset(mRegistry, entity);
  //  sys::remove_tileset(mRegistry, mTilesetId);
}

}  // namespace tactile
