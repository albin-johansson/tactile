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

#include "set_animation_frame_duration.hpp"

#include "common/debug/panic.hpp"
#include "core/tile/tile.hpp"
#include "lang/language.hpp"
#include "lang/strings.hpp"
#include "model/document/tileset_document.hpp"

namespace tactile::cmd {

SetAnimationFrameDuration::SetAnimationFrameDuration(TilesetDocument* document,
                                                     const TileIndex tile_index,
                                                     const usize frame_index,
                                                     const ms_t frame_duration)
    : mDocument {document},
      mTileIndex {tile_index},
      mFrameIndex {frame_index},
      mNewFrameDuration {frame_duration}
{
  if (!mDocument) {
    throw TactileError {"Invalid null tileset document"};
  }
}

void SetAnimationFrameDuration::undo()
{
  auto& tileset = mDocument->get_tileset();
  auto& tile = tileset[mTileIndex];

  auto& animation = tile.get_animation();
  auto& frame = animation[mFrameIndex];

  frame.duration = mOldFrameDuration.value();
  mOldFrameDuration.reset();
}

void SetAnimationFrameDuration::redo()
{
  auto& tileset = mDocument->get_tileset();
  auto& tile = tileset[mTileIndex];

  auto& animation = tile.get_animation();
  auto& frame = animation[mFrameIndex];

  mOldFrameDuration = frame.duration;
  frame.duration = mNewFrameDuration;
}

auto SetAnimationFrameDuration::merge_with(const Command* cmd) -> bool
{
  if (const auto* other = dynamic_cast<const SetAnimationFrameDuration*>(cmd)) {
    if (mTileIndex == other->mTileIndex && mFrameIndex == other->mFrameIndex) {
      mNewFrameDuration = other->mNewFrameDuration;
      return true;
    }
  }

  return false;
}

auto SetAnimationFrameDuration::get_name() const -> String
{
  const auto& lang = get_current_language();
  return lang.cmd.set_animation_frame_duration;
}

}  // namespace tactile::cmd
