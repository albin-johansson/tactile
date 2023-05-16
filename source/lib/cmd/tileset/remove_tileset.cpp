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

#include "core/map.hpp"
#include "core/tileset.hpp"
#include "lang/language.hpp"
#include "lang/strings.hpp"
#include "model/context.hpp"
#include "model/systems/document_system.hpp"

namespace tactile::cmd {

RemoveTileset::RemoveTileset(const Entity map_entity,
                             const Entity attached_tileset_entity)
    : mMapEntity {map_entity},
      mAttachedTilesetEntity {attached_tileset_entity}
{
}

void RemoveTileset::undo()
{
  auto& model = get_global_model();

  auto& map = model.get<Map>(mMapEntity);
  map.attached_tilesets.push_back(mAttachedTilesetEntity);
  map.active_tileset = mAttachedTilesetEntity;

  model.set_enabled(mAttachedTilesetEntity, true);
  mDidDetachTileset = false;
}

void RemoveTileset::redo()
{
  auto& model = get_global_model();

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
    auto& model = get_global_model();
    model.destroy(mAttachedTilesetEntity);
  }
}

auto RemoveTileset::get_name() const -> String
{
  const auto& lang = get_current_language();
  return lang.cmd.remove_tileset;
}

}  // namespace tactile::cmd
