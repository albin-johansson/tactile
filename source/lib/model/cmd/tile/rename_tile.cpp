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

#include "rename_tile.hpp"

#include <utility>  // move

#include "common/debug/panic.hpp"
#include "core/tile/tile.hpp"
#include "lang/language.hpp"
#include "lang/strings.hpp"
#include "model/document/tileset_document.hpp"

namespace tactile::cmd {

RenameTile::RenameTile(TilesetDocument* document, const TileIndex tile_index, String name)
    : mDocument {document},
      mTileIndex {tile_index},
      mNewName {std::move(name)}
{
  if (!mDocument) {
    throw TactileError {"Invalid null tileset document"};
  }
}

void RenameTile::undo()
{
  auto& tileset = mDocument->get_tileset();
  auto& tile = tileset[mTileIndex];
  auto& tile_ctx = tile.get_ctx();

  tile_ctx.set_name(mOldName.value());
  mOldName.reset();
}

void RenameTile::redo()
{
  auto& tileset = mDocument->get_tileset();
  auto& tile = tileset[mTileIndex];
  auto& tile_ctx = tile.get_ctx();

  mOldName = tile_ctx.name();
  tile_ctx.set_name(mNewName);
}

auto RenameTile::get_name() const -> String
{
  const auto& lang = get_current_language();
  return lang.cmd.rename_tile;
}

}  // namespace tactile::cmd
