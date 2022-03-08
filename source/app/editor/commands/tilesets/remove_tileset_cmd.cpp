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

#include "remove_tileset_cmd.hpp"

#include "core/systems/tileset_system.hpp"
#include "misc/assert.hpp"

namespace tactile {

remove_tileset_cmd::remove_tileset_cmd(registry_ref registry, const tileset_id id)
    : ACommand{"Remove Tileset"}
    , mRegistry{registry}
    , mTilesetId{id}
{}

void remove_tileset_cmd::undo()
{
  auto& registry = mRegistry.get();
  sys::restore_tileset(registry, mSnapshot.value());

  auto& active = registry.ctx<comp::active_tileset>();
  active.entity = sys::find_tileset(registry, mTilesetId);
}

void remove_tileset_cmd::redo()
{
  const auto entity = sys::find_tileset(mRegistry, mTilesetId);
  TACTILE_ASSERT(entity != entt::null);

  mSnapshot = sys::copy_tileset(mRegistry, entity);
  sys::remove_tileset(mRegistry, mTilesetId);
}

}  // namespace tactile
