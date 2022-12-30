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

#include "delete_animation.hpp"

#include "core/debug/panic.hpp"
#include "core/tile/tile.hpp"
#include "lang/language.hpp"
#include "lang/strings.hpp"
#include "model/document/tileset_document.hpp"

namespace tactile::cmd {

DeleteAnimation::DeleteAnimation(TilesetDocument* document, const TileIndex tile_index)
    : mDocument {document},
      mTileIndex {tile_index}
{
  if (!mDocument) {
    throw TactileError {"Invalid null tileset document"};
  }
}

void DeleteAnimation::undo()
{
  auto& tileset = mDocument->get_tileset();

  auto& tile = tileset[mTileIndex];
  tile.set_animation(mRemovedAnimation.value());

  mRemovedAnimation.reset();
}

void DeleteAnimation::redo()
{
  auto& tileset = mDocument->get_tileset();
  auto& tile = tileset[mTileIndex];

  mRemovedAnimation = tile.get_animation();
  tile.clear_animation();
}

auto DeleteAnimation::get_name() const -> String
{
  const auto& lang = get_current_language();
  return lang.cmd.delete_animation;
}

}  // namespace tactile::cmd
