/*
 * This source file is a part of the Tactile map editor.
 *
 * Copyright (C) 2023 Albin Johansson
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

#include "create_tileset.hpp"

#include <memory>   // make_shared
#include <utility>  // move

#include "common/debug/panic.hpp"
#include "core/tile/tileset_bundle.hpp"
#include "lang/language.hpp"
#include "lang/strings.hpp"
#include "model/document/map_document.hpp"
#include "model/document/tileset_document.hpp"
#include "model/model.hpp"
#include "tactile/core/debug/assert.hpp"

namespace tactile::cmd {

CreateTileset::CreateTileset(DocumentModel* model,
                             const UUID& map_id,
                             const UUID& tileset_id,
                             TilesetInfo info)
    : mModel {model},
      mMapId {map_id},
      mTilesetId {tileset_id},
      mTilesetInfo {std::move(info)}
{
  if (!mModel) {
    throw TactileError {"Invalid null model!"};
  }
}

void CreateTileset::undo()
{
  if (mModel->is_open(mTilesetId)) {
    mModel->close_document(mTilesetId);
  }

  TACTILE_ASSERT(!mModel->is_open(mTilesetId));
  TACTILE_ASSERT(mModel->is_tileset(mTilesetId));

  auto document = mModel->get_map_document_ptr(mMapId);

  auto& tilesets = document->get_map().get_tileset_bundle();
  tilesets.detach_tileset(mTilesetId);

  document->get_contexts().remove_context(mTilesetId);
}

void CreateTileset::redo()
{
  // We only need to create and register the document once
  if (!mTileset) {
    mTileset = std::make_shared<TilesetDocument>(mTilesetInfo, mTilesetId);
    mTileset->set_name(mTilesetInfo.texture->get_path().stem().string());
    mModel->register_tileset(mTileset);
  }

  TACTILE_ASSERT(mModel->is_tileset(mTilesetId));
  TACTILE_ASSERT(mModel->is_map(mMapId));

  auto document = mModel->get_map_document_ptr(mMapId);
  auto& map = document->get_map();

  auto tileset = mTileset->get_tileset_ptr();
  map.get_tileset_bundle().attach_tileset(tileset, false);

  document->get_contexts().add_context(tileset);
}

auto CreateTileset::get_name() const -> String
{
  const auto& lang = get_current_language();
  return lang.cmd.add_tileset;
}

}  // namespace tactile::cmd
