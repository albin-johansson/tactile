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

#include "rename_tileset_cmd.hpp"

#include <utility>  // move

#include "core/components/attributes.hpp"
#include "core/systems/registry_system.hpp"
#include "core/systems/tileset_system.hpp"
#include "misc/assert.hpp"

namespace tactile {

RenameTilesetCmd::RenameTilesetCmd(RegistryRef registry,
                                   const TilesetID id,
                                   std::string name)
    : ACommand{"Rename Tileset"}
    , mRegistry{registry}
    , mTilesetId{id}
    , mNewName{std::move(name)}
{}

void RenameTilesetCmd::undo()
{
  auto& registry = mRegistry.get();

  const auto entity = sys::find_tileset(registry, mTilesetId);
  TACTILE_ASSERT(entity != entt::null);

  auto& context = sys::checked_get<comp::AttributeContext>(registry, entity);
  context.name = mOldName.value();
}

void RenameTilesetCmd::redo()
{
  auto& registry = mRegistry.get();

  const auto entity = sys::find_tileset(registry, mTilesetId);
  TACTILE_ASSERT(entity != entt::null);

  auto& context = sys::checked_get<comp::AttributeContext>(registry, entity);
  mOldName = context.name;
  context.name = mNewName;
}

auto RenameTilesetCmd::merge_with(const ACommand& cmd) -> bool
{
  if (id() == cmd.id()) {
    const auto& other = dynamic_cast<const RenameTilesetCmd&>(cmd);
    if (mTilesetId == other.mTilesetId) {
      mNewName = other.mNewName;
      return true;
    }
  }

  return false;
}

}  // namespace tactile
