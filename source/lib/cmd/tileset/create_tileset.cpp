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

#include "common/debug/assert.hpp"
#include "model/contexts/context_components.hpp"
#include "model/documents/document_components.hpp"
#include "model/documents/document_factory.hpp"
#include "model/documents/document_system.hpp"
#include "model/entity_validation.hpp"
#include "model/i18n/language_system.hpp"
#include "model/maps/map_components.hpp"
#include "model/maps/map_system.hpp"

namespace tactile::cmd {

CreateTileset::CreateTileset(Registry* registry,
                             const Entity map_entity,
                             const Int2 tile_size,
                             Path image_path)
    : mRegistry {registry},
      mMapEntity {map_entity},
      mTileSize {tile_size},
      mImagePath {std::move(image_path)}
{
  TACTILE_ASSERT(sys::is_map_entity(*mRegistry, mMapEntity));
}

void CreateTileset::undo()
{
  auto& registry = *mRegistry;

  // TODO in the future, we should check if there are other maps that use the tileset
  if (sys::is_document_open(registry, mTilesetDocumentEntity)) {
    sys::close_document(registry, mTilesetDocumentEntity);
  }

  auto& map = registry.get<Map>(mMapEntity);
  std::erase(map.attached_tilesets, mAttachedTilesetEntity);

  registry.set_enabled(mAttachedTilesetEntity, false);
  mHasAttachedTileset = false;
}

void CreateTileset::redo()
{
  auto& registry = *mRegistry;

  if (mTilesetDocumentEntity == kNullEntity) {
    _create_tileset_document();
  }

  if (mAttachedTilesetEntity != kNullEntity) {
    auto& map = registry.get<Map>(mMapEntity);
    map.attached_tilesets.push_back(mAttachedTilesetEntity);
  }
  else {
    const auto& tileset_document = registry.get<TilesetDocument>(mTilesetDocumentEntity);
    mAttachedTilesetEntity =
        sys::attach_tileset_to_map(registry, mMapEntity, tileset_document.tileset);
  }

  registry.set_enabled(mAttachedTilesetEntity, true);
  mHasAttachedTileset = true;
}

void CreateTileset::dispose()
{
  auto& registry = *mRegistry;

  if (!mHasAttachedTileset) {
    registry.destroy(mAttachedTilesetEntity);
    sys::destroy_document(registry, mTilesetDocumentEntity);
  }
}

void CreateTileset::_create_tileset_document()
{
  auto& registry = *mRegistry;

  mTilesetDocumentEntity = sys::create_tileset_document(registry, mTileSize, mImagePath);
  const auto& tileset_document = registry.get<TilesetDocument>(mTilesetDocumentEntity);

  auto& tileset_context = registry.get<Context>(tileset_document.tileset);
  tileset_context.name = mImagePath.stem().string();
}

auto CreateTileset::get_name() const -> String
{
  const auto& strings = sys::get_current_language_strings(*mRegistry);
  return strings.cmd.add_tileset;
}

}  // namespace tactile::cmd
