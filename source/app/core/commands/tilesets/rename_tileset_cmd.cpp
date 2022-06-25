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

#include "core/documents/tileset_document.hpp"
#include "misc/panic.hpp"

namespace tactile {

RenameTilesetCmd::RenameTilesetCmd(TilesetDocument* document, std::string name)
    : ACommand{"Rename Tileset"}
    , mDocument{document}
    , mNewName{std::move(name)}
{
  if (!mDocument) {
    throw TactileError{"Invalid tileset null document!"};
  }
}

void RenameTilesetCmd::undo()
{
  auto& tileset = mDocument->view_tileset();

  tileset.set_name(mOldName.value());
  mOldName.reset();
}

void RenameTilesetCmd::redo()
{
  auto& tileset = mDocument->view_tileset();

  mOldName = tileset.get_name();
  tileset.set_name(mNewName);
}

auto RenameTilesetCmd::merge_with(const ACommand& cmd) -> bool
{
  if (id() == cmd.id()) {
    const auto& other = dynamic_cast<const RenameTilesetCmd&>(cmd);

    const auto& tileset = mDocument->view_tileset();
    const auto& otherTileset = other.mDocument->view_tileset();

    if (tileset.get_uuid() == otherTileset.get_uuid()) {
      mNewName = other.mNewName;
      return true;
    }
  }

  return false;
}

}  // namespace tactile
