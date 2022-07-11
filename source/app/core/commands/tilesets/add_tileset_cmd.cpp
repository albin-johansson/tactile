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

#include "core/documents/map_document.hpp"
#include "core/documents/tileset_document.hpp"
#include "core/model.hpp"
#include "misc/assert.hpp"
#include "misc/panic.hpp"

namespace tactile {

AddTilesetCmd::AddTilesetCmd(DocumentModel*     model,
                             const UUID&        mapId,
                             const UUID&        tilesetId,
                             const TilesetInfo& info)
    : mModel {model}
    , mMapId {mapId}
    , mTilesetId {tilesetId}
    , mTilesetInfo {info}
{
  if (!mModel) {
    throw TactileError {"Invalid null model!"};
  }
}

void AddTilesetCmd::undo()
{
  if (mModel->is_open(mTilesetId)) {
    mModel->close_document(mTilesetId);
  }

  TACTILE_ASSERT(!mModel->is_open(mTilesetId));
  TACTILE_ASSERT(mModel->is_tileset(mTilesetId));

  auto  document = mModel->get_map(mMapId);
  auto& map = document->get_map();
  map.detach_tileset(mTilesetId);

  document->unregister_context(mTilesetId);
}

void AddTilesetCmd::redo()
{
  /* We only need to create and register the document once */
  if (!mTileset) {
    mTileset = std::make_shared<TilesetDocument>(mTilesetId, mTilesetInfo);
    mModel->register_tileset(mTileset);
  }

  TACTILE_ASSERT(mModel->is_tileset(mTilesetId));
  TACTILE_ASSERT(mModel->is_map(mMapId));

  auto  document = mModel->get_map(mMapId);
  auto& map = document->get_map();

  auto tileset = mTileset->get_tileset();
  map.attach_tileset(tileset, false);

  document->register_context(tileset);
}

auto AddTilesetCmd::get_name() const -> const char*
{
  return "Add Tileset";
}

}  // namespace tactile
