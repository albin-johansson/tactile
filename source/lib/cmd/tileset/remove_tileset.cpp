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

#include "model/documents/document_system.hpp"
#include "model/entity_validation.hpp"
#include "model/maps/map_components.hpp"
#include "model/services/language_service.hpp"
#include "model/services/service_locator.hpp"
#include "model/tilesets/tileset_components.hpp"
#include "tactile/core/common/assert.hpp"

namespace tactile::cmd {

RemoveTileset::RemoveTileset(Registry* registry,
                             const Entity map_entity,
                             const Entity attached_tileset_entity)
    : mRegistry {registry},
      mMapEntity {map_entity},
      mAttachedTilesetEntity {attached_tileset_entity}
{
  TACTILE_ASSERT(sys::is_map_entity(*mRegistry, mMapEntity));
  TACTILE_ASSERT(sys::is_attached_tileset_entity(*mRegistry, mAttachedTilesetEntity));
}

void RemoveTileset::undo()
{
  auto& registry = *mRegistry;

  auto& map = registry.get<Map>(mMapEntity);
  map.attached_tilesets.push_back(mAttachedTilesetEntity);
  map.active_tileset = mAttachedTilesetEntity;

  registry.set_enabled(mAttachedTilesetEntity, true);
  mDidDetachTileset = false;
}

void RemoveTileset::redo()
{
  auto& registry = *mRegistry;

  auto& map = registry.get<Map>(mMapEntity);
  std::erase(map.attached_tilesets, mAttachedTilesetEntity);

  const auto& attached_tileset = registry.get<AttachedTileset>(mAttachedTilesetEntity);
  if (attached_tileset.embedded) {
    const auto tileset_document_entity =
        sys::get_associated_tileset_document(registry, attached_tileset.tileset);
    sys::close_document(registry, tileset_document_entity);
  }

  registry.set_enabled(mAttachedTilesetEntity, false);
  mDidDetachTileset = true;
}

void RemoveTileset::dispose()
{
  if (mDidDetachTileset) {
    mRegistry->destroy(mAttachedTilesetEntity);
  }
}

auto RemoveTileset::get_name() const -> String
{
  const auto& strings =
      ServiceLocator<LanguageService>::get().get_current_language_strings();
  return strings.cmd.remove_tileset;
}

}  // namespace tactile::cmd
