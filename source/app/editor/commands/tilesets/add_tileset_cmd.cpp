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

#include <utility>  // move

#include "core/systems/tileset_system.hpp"
#include "misc/assert.hpp"

namespace tactile {

AddTilesetCmd::AddTilesetCmd(RegistryRef registry,
                             comp::Texture texture,
                             const int32 tileWidth,
                             const int32 tileHeight)
    : ACommand{"Add Tileset"}
    , mRegistry{registry}
    , mTexture{std::move(texture)}
    , mTileWidth{tileWidth}
    , mTileHeight{tileHeight}
{}

void AddTilesetCmd::undo()
{
  auto& registry = mRegistry.get();
  const auto id = mTilesetId.value();

  const auto entity = sys::find_tileset(registry, id);
  TACTILE_ASSERT(entity != entt::null);

  mSnapshot = sys::copy_tileset(registry, entity);
  sys::remove_tileset(registry, id);
}

void AddTilesetCmd::redo()
{
  auto& registry = mRegistry.get();
  if (!mSnapshot) {
    const auto entity = sys::make_tileset(registry, mTexture, mTileWidth, mTileHeight);
    const auto& tileset = registry.get<comp::Tileset>(entity);
    mTilesetId = tileset.id;
  }
  else {
    sys::restore_tileset(registry, *mSnapshot);
  }

  auto& active = registry.ctx().at<comp::ActiveTileset>();
  active.entity = sys::find_tileset(registry, mTilesetId.value());
}

}  // namespace tactile
