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

#include "remove_tileset.hpp"

#include <utility>  // move

#include "common/debug/assert.hpp"
#include "core/tile/tileset_bundle.hpp"
#include "lang/language.hpp"
#include "lang/strings.hpp"
#include "model/document/map_document.hpp"
#include "model/document/tileset_document.hpp"
#include "model/model.hpp"
#include "tactile/core/debug/error.hpp"

namespace tactile::cmd {

RemoveTileset::RemoveTileset(DocumentModel* model, const UUID& tileset_id)
    : mModel {model},
      mTilesetId {tileset_id}
{
  if (!mModel) {
    throw Error {"Invalid null model!"};
  }

  TACTILE_ASSERT(mModel->get_active_document_id().has_value());
  mMapId = mModel->get_active_document_id().value();
}

void RemoveTileset::undo()
{
  TACTILE_ASSERT(mTilesetDocument != nullptr);
  TACTILE_ASSERT(mModel->get_active_document_id() == mMapId);

  auto map_document = mModel->get_map_document_ptr(mMapId);
  auto& map = map_document->get_map();

  auto tileset = mTilesetDocument->get_tileset_ptr();
  const auto tileset_id = tileset->get_uuid();

  auto& tilesets = map.get_tileset_bundle();
  tilesets.attach_tileset(tileset, mFirstTile.value(), false);  // TODO
  tilesets.select_tileset(tileset_id);

  map_document->get_contexts().add_context(std::move(tileset));
}

void RemoveTileset::redo()
{
  TACTILE_ASSERT(mModel->get_active_document_id() == mMapId);
  mTilesetDocument = mModel->get_tileset_document_ptr(mTilesetId);

  auto map_document = mModel->get_map_document_ptr(mMapId);
  auto& map = map_document->get_map();

  auto& tilesets = map.get_tileset_bundle();
  const auto& tileset_ref = tilesets.get_tileset_ref(mTilesetId);
  mFirstTile = tileset_ref.get_first_tile();

  if (tileset_ref.is_embedded()) {
    mModel->close_document(mTilesetId);
  }

  tilesets.detach_tileset(mTilesetId);
  map_document->get_contexts().remove_context(mTilesetId);
}

auto RemoveTileset::get_name() const -> String
{
  const auto& lang = get_current_language();
  return lang.cmd.remove_tileset;
}

}  // namespace tactile::cmd
