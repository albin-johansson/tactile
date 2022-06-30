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

#include <utility>  // move

#include "core/documents/map_document.hpp"
#include "core/documents/tileset_document.hpp"
#include "core/model.hpp"
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
  TACTILE_ASSERT(mTilesetDocument != nullptr);
  TACTILE_ASSERT(mModel->active_document_id() == mMapId);

  auto  mapDocument = mModel->get_map(mMapId);
  auto& map = mapDocument->get_map();

  auto       tileset = mTilesetDocument->get_tileset();
  const auto tilesetId = tileset->get_uuid();

  map.attach_tileset(tileset, mFirstTile.value(), false);  // TODO
  map.select_tileset(tilesetId);

  mapDocument->register_context(std::move(tileset));
}

void RemoveTilesetCmd::redo()
{
  TACTILE_ASSERT(mModel->active_document_id() == mMapId);
  mTilesetDocument = mModel->get_tileset(mTilesetId);

  auto  mapDocument = mModel->get_map(mMapId);
  auto& map = mapDocument->get_map();

  const auto& tilesets = map.get_tilesets();
  const auto& tilesetRef = tilesets.get_ref(mTilesetId);
  mFirstTile = tilesetRef.first_tile;

  if (tilesetRef.embedded) {
    mModel->close_document(mTilesetId);
  }

  map.detach_tileset(mTilesetId);
  mapDocument->unregister_context(mTilesetId);
}

}  // namespace tactile
