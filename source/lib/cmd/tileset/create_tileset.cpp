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

#include <utility>  // move

#include "components/context.hpp"
#include "components/document.hpp"
#include "components/map.hpp"
#include "model/context.hpp"
#include "model/systems/document_system.hpp"
#include "model/systems/map_system.hpp"
#include "systems/language_system.hpp"

namespace tactile::cmd {

CreateTileset::CreateTileset(const Entity map_entity,
                             const Int2 tile_size,
                             Path image_path)
    : mMapEntity {map_entity},
      mTileSize {tile_size},
      mImagePath {std::move(image_path)}
{
}

void CreateTileset::undo()
{
  auto& model = get_global_model();

  const auto tileset_document_entity = mTilesetDocumentEntity.value();
  const auto attached_tileset_entity = mAttachedTilesetEntity.value();

  sys::close_document(model, tileset_document_entity);

  auto& map = model.get<Map>(mMapEntity);
  std::erase(map.attached_tilesets, attached_tileset_entity);

  model.set_enabled(attached_tileset_entity, false);
  mHasAttachedTileset = false;
}

void CreateTileset::redo()
{
  if (!mTilesetDocumentEntity.has_value()) {
    create_tileset_document();
  }

  auto& model = get_global_model();

  if (mAttachedTilesetEntity.has_value()) {
    auto& map = model.get<Map>(mMapEntity);
    map.attached_tilesets.push_back(*mAttachedTilesetEntity);
  }
  else {
    const auto& tileset_document = model.get<TilesetDocument>(*mTilesetDocumentEntity);
    mAttachedTilesetEntity =
        sys::attach_tileset_to_map(model, mMapEntity, tileset_document.tileset);
  }

  model.set_enabled(mAttachedTilesetEntity.value(), true);
  mHasAttachedTileset = true;
}

void CreateTileset::dispose()
{
  if (!mHasAttachedTileset) {
    auto& model = get_global_model();

    model.destroy(mAttachedTilesetEntity.value());
    sys::destroy_document(model, mTilesetDocumentEntity.value());
  }
}

void CreateTileset::create_tileset_document()
{
  auto& model = get_global_model();
  mTilesetDocumentEntity = sys::create_tileset_document(model, mTileSize, mImagePath);

  const auto& tileset_document = model.get<TilesetDocument>(*mTilesetDocumentEntity);
  auto& tileset_context = model.get<Context>(tileset_document.tileset);
  tileset_context.name = mImagePath.stem().string();
}

auto CreateTileset::get_name() const -> String
{
  const auto& strings = sys::get_current_language_strings(get_global_model());
  return strings.cmd.add_tileset;
}

}  // namespace tactile::cmd
