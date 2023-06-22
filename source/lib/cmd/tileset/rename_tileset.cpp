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

#include "rename_tileset.hpp"

#include <utility>  // move

#include "common/debug/assert.hpp"
#include "model/contexts/context_components.hpp"
#include "model/entity_validation.hpp"
#include "model/systems/language_system.hpp"

namespace tactile::cmd {

RenameTileset::RenameTileset(Model* model, const Entity tileset_entity, String new_name)
    : mModel {model},
      mTilesetEntity {tileset_entity},
      mNewName {std::move(new_name)}
{
  TACTILE_ASSERT(sys::is_tileset_entity(*mModel, mTilesetEntity));
}

void RenameTileset::undo()
{
  auto& model = *mModel;

  auto& tileset_context = model.get<Context>(mTilesetEntity);
  tileset_context.name = mOldName.value();

  mOldName.reset();
}

void RenameTileset::redo()
{
  auto& model = *mModel;
  auto& tileset_context = model.get<Context>(mTilesetEntity);

  mOldName = tileset_context.name;
  tileset_context.name = mNewName;
}

auto RenameTileset::merge_with(const Command* cmd) -> bool
{
  if (const auto* other = dynamic_cast<const RenameTileset*>(cmd)) {
    if (mTilesetEntity == other->mTilesetEntity) {
      mNewName = other->mNewName;
      return true;
    }
  }

  return false;
}

auto RenameTileset::get_name() const -> String
{
  const auto& strings = sys::get_current_language_strings(*mModel);
  return strings.cmd.rename_tileset;
}

}  // namespace tactile::cmd
