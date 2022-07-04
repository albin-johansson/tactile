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

#include "core/tilesets/tileset.hpp"
#include "misc/panic.hpp"

namespace tactile {

RenameTilesetCmd::RenameTilesetCmd(Shared<core::Tileset> tileset, std::string name)
    : mTileset{std::move(tileset)}
    , mNewName{std::move(name)}
{
  if (!mTileset) {
    throw TactileError{"Invalid null tileset!"};
  }
}

void RenameTilesetCmd::undo()
{
  mTileset->set_name(mOldName.value());
  mOldName.reset();
}

void RenameTilesetCmd::redo()
{
  mOldName = mTileset->get_name();
  mTileset->set_name(mNewName);
}

auto RenameTilesetCmd::merge_with(const ICommand* cmd) -> bool
{
  if (const auto* other = dynamic_cast<const RenameTilesetCmd*>(cmd)) {
    if (mTileset->get_uuid() == other->mTileset->get_uuid()) {
      mNewName = other->mNewName;
      return true;
    }
  }

  return false;
}

auto RenameTilesetCmd::get_name() const -> const char*
{
  return "Rename Tileset";
}

}  // namespace tactile
