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

#include "set_tileset_name_cmd.hpp"

#include <utility>  // move

#include "core/components/attribute_context.hpp"
#include "core/systems/tileset_system.hpp"
#include "misc/assert.hpp"

namespace tactile {

set_tileset_name_cmd::set_tileset_name_cmd(registry_ref registry,
                                           const tileset_id id,
                                           std::string name)
    : command_base{"Set Tileset Name"}
    , mRegistry{registry}
    , mTilesetId{id}
    , mNewName{std::move(name)}
{}

void set_tileset_name_cmd::undo()
{
  auto& registry = mRegistry.get();

  const auto entity = sys::find_tileset(registry, mTilesetId);
  TACTILE_ASSERT(entity != entt::null);

  auto& context = registry.get<comp::attribute_context>(entity);
  context.name = mOldName.value();
}

void set_tileset_name_cmd::redo()
{
  auto& registry = mRegistry.get();

  const auto entity = sys::find_tileset(registry, mTilesetId);
  TACTILE_ASSERT(entity != entt::null);

  auto& context = registry.get<comp::attribute_context>(entity);
  mOldName = context.name;
  context.name = mNewName;
}

auto set_tileset_name_cmd::merge_with(const command_base& cmd) -> bool
{
  if (id() == cmd.id()) {
    const auto& other = dynamic_cast<const set_tileset_name_cmd&>(cmd);
    if (mTilesetId == other.mTilesetId) {
      mNewName = other.mNewName;
      return true;
    }
  }

  return false;
}

}  // namespace tactile
