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

#include "components/tile.hpp"
#include "model/context.hpp"
#include "model/systems/language_system.hpp"

namespace tactile::cmd {

SetAnimationFrameDuration::SetAnimationFrameDuration(const Entity tile_entity,
                                                     const usize frame_index,
                                                     const ms_t new_frame_duration)
    : mTileEntity {tile_entity},
      mFrameIndex {frame_index},
      mNewFrameDuration {new_frame_duration}
{
}

void SetAnimationFrameDuration::undo()
{
  auto& model = get_global_model();
  auto& animation = model.get<TileAnimation>(mTileEntity);

  auto& frame = animation.frames.at(mFrameIndex);
  frame.duration = mOldFrameDuration.value();

  mOldFrameDuration.reset();
}

void SetAnimationFrameDuration::redo()
{
  auto& model = get_global_model();

  auto& animation = model.get<TileAnimation>(mTileEntity);
  auto& frame = animation.frames.at(mFrameIndex);

  mOldFrameDuration = frame.duration;
  frame.duration = mNewFrameDuration;
}

auto SetAnimationFrameDuration::merge_with(const Command* cmd) -> bool
{
  if (const auto* other = dynamic_cast<const SetAnimationFrameDuration*>(cmd)) {
    if (mTileEntity == other->mTileEntity && mFrameIndex == other->mFrameIndex) {
      mNewFrameDuration = other->mNewFrameDuration;
      return true;
    }
  }

  return false;
}

auto SetAnimationFrameDuration::get_name() const -> String
{
  const auto& strings = sys::get_current_language_strings(get_global_model());
  return strings.cmd.set_animation_frame_duration;
}

}  // namespace tactile::cmd
