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

#include "remove_animation_frame.hpp"

#include "common/debug/panic.hpp"
#include "core/tile/tile.hpp"
#include "lang/language.hpp"
#include "lang/strings.hpp"
#include "model/document/tileset_document.hpp"

namespace tactile::cmd {

RemoveAnimationFrame::RemoveAnimationFrame(TilesetDocument* document,
                                           const TileIndex tile_index,
                                           const usize frame_index)
    : mDocument {document},
      mTileIndex {tile_index},
      mFrameIndex {frame_index}
{
  if (!mDocument) {
    throw TactileError {"Invalid null tileset document"};
  }
}

void RemoveAnimationFrame::undo()
{
  auto& tileset = mDocument->get_tileset();
  auto& tile = tileset[mTileIndex];

  if (mRemovedAnimation.has_value()) {
    tile.set_animation(*mRemovedAnimation);
  }

  auto& animation = tile.get_animation();
  animation.insert_frame(mFrameIndex, mFrameTileIndex.value(), mFrameDuration.value());

  mFrameTileIndex.reset();
  mFrameDuration.reset();
}

void RemoveAnimationFrame::redo()
{
  auto& tileset = mDocument->get_tileset();
  auto& tile = tileset[mTileIndex];
  auto& animation = tile.get_animation();

  {
    const auto& frame = animation[mFrameIndex];
    mFrameTileIndex = frame.tile;
    mFrameDuration = frame.duration;
  }

  animation.remove_frame(mFrameIndex);

  // Check for case when only the animated tile itself is in the sequence
  if (animation.size() == 1 && animation[0].tile == mTileIndex) {
    mRemovedAnimation = animation;
    tile.clear_animation();
  }
  else if (animation.size() == 0) {
    tile.clear_animation();
  }
}

auto RemoveAnimationFrame::get_name() const -> String
{
  const auto& lang = get_current_language();
  return lang.cmd.remove_animation_frame;
}

}  // namespace tactile::cmd
