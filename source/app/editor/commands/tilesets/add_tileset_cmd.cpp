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

#include "core/components/attributes.hpp"
#include "core/systems/tileset_system.hpp"
#include "core/systems/tilesets/tileset_document_system.hpp"
#include "editor/documents/map_document.hpp"
#include "editor/documents/tileset_document.hpp"
#include "editor/model.hpp"
#include "misc/assert.hpp"
#include "misc/panic.hpp"

namespace tactile {

AddTilesetCmd::AddTilesetCmd(DocumentModel* model,
                             const UUID& mapId,
                             const UUID& tilesetId,
                             comp::Texture texture,
                             const glm::ivec2& tileSize)
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
  // TODO
  //  auto& registry = mModel->document_registry();
  //  auto& mapDocument = mModel->get_document(mMapId);
  //
  //  for (auto&& [entity, ref] : mapDocument.registry.view<comp::TilesetRef>().each()) {
  //    if (ref.source_tileset == mTilesetId) {
  //      mapDocument.registry.destroy(entity);
  //      break;
  //    }
  //  }
  //
  //  for (auto&& [entity, tileset, context] :
  //       registry.view<comp::Tileset, comp::AttributeContext>().each()) {
  //    if (context.id == mTilesetId) {
  //      registry.destroy(entity);
  //      break;
  //    }
  //  }
}

void AddTilesetCmd::redo()
{
  if (!mTileset) {
    mTileset = std::make_shared<TilesetDocument>(mTilesetId, mTexture, mTileSize);
  }
  mModel->register_tileset(mTileset);

  auto mapDocument = mModel->get_map(mMapId);
  sys::attach_tileset(mapDocument->get_registry(), mTilesetId, mTileset->info());
}

}  // namespace tactile
