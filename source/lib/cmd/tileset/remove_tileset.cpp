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

#include "common/debug/assert.hpp"
#include "model/components/map.hpp"
#include "model/documents/document_system.hpp"
#include "model/systems/language_system.hpp"
#include "model/systems/validation_system.hpp"
#include "model/tilesets/tileset_components.hpp"

namespace tactile::cmd {

RemoveTileset::RemoveTileset(Model* model,
                             const Entity map_entity,
                             const Entity attached_tileset_entity)
    : mModel {model},
      mMapEntity {map_entity},
      mAttachedTilesetEntity {attached_tileset_entity}
{
  TACTILE_ASSERT(sys::is_map_entity(*mModel, mMapEntity));
  TACTILE_ASSERT(sys::is_attached_tileset_entity(*mModel, mAttachedTilesetEntity));
}

void RemoveTileset::undo()
{
  auto& model = *mModel;

  auto& map = model.get<Map>(mMapEntity);
  map.attached_tilesets.push_back(mAttachedTilesetEntity);
  map.active_tileset = mAttachedTilesetEntity;

  model.set_enabled(mAttachedTilesetEntity, true);
  mDidDetachTileset = false;
}

void RemoveTileset::redo()
{
  auto& model = *mModel;

  auto& map = model.get<Map>(mMapEntity);
  std::erase(map.attached_tilesets, mAttachedTilesetEntity);

  const auto& attached_tileset = model.get<AttachedTileset>(mAttachedTilesetEntity);
  if (attached_tileset.embedded) {
    const auto tileset_document_entity =
        sys::get_associated_tileset_document(model, attached_tileset.tileset);
    sys::close_document(model, tileset_document_entity);
  }

  model.set_enabled(mAttachedTilesetEntity, false);
  mDidDetachTileset = true;
}

void RemoveTileset::dispose()
{
  if (mDidDetachTileset) {
    mModel->destroy(mAttachedTilesetEntity);
  }
}

auto RemoveTileset::get_name() const -> String
{
  const auto& strings = sys::get_current_language_strings(*mModel);
  return strings.cmd.remove_tileset;
}

}  // namespace tactile::cmd
