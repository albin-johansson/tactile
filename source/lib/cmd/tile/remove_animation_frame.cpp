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

#include "remove_animation_frame.hpp"

#include "model/context.hpp"
#include "model/systems/language_system.hpp"

namespace tactile::cmd {

RemoveAnimationFrame::RemoveAnimationFrame(const Entity tile_entity,
                                           const usize frame_index)
    : mTileEntity {tile_entity},
      mFrameIndex {frame_index}
{
}

void RemoveAnimationFrame::undo()
{
  auto& model = get_global_model();

  if (mRemovedAnimation.has_value()) {
    auto& animation = model.add<TileAnimation>(mTileEntity);
    animation = *mRemovedAnimation;
  }

  auto& animation = model.get<TileAnimation>(mTileEntity);
  animation.frames.insert(animation.frames.begin() + static_cast<ssize>(mFrameIndex),
                          TileAnimationFrame {
                              .tile_index = mFrameTileIndex.value(),
                              .duration = mFrameDuration.value(),
                          });

  mFrameTileIndex.reset();
  mFrameDuration.reset();
}

void RemoveAnimationFrame::redo()
{
  auto& model = get_global_model();

  const auto& tile = model.get<Tile>(mTileEntity);
  auto& animation = model.get<TileAnimation>(mTileEntity);

  {
    const auto& frame = animation.frames.at(mFrameIndex);
    mFrameTileIndex = frame.tile_index;
    mFrameDuration = frame.duration;
  }

  animation.frames.erase(animation.frames.begin() + static_cast<ssize>(mFrameIndex));

  // Check if the animated tile is itself in the sequence
  if (animation.frames.size() == 1 && animation.frames.at(0).tile_index == tile.index) {
    mRemovedAnimation = animation;
    model.remove<TileAnimation>(mTileEntity);
  }
  else if (animation.frames.empty()) {
    model.remove<TileAnimation>(mTileEntity);
  }
}

auto RemoveAnimationFrame::get_name() const -> String
{
  const auto& strings = sys::get_current_language_strings(get_global_model());
  return strings.cmd.remove_animation_frame;
}

}  // namespace tactile::cmd
