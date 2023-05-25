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

#include "move_animation_frame_forwards.hpp"

#include "common/util/vectors.hpp"
#include "components/tile.hpp"
#include "model/context.hpp"
#include "model/systems/language_system.hpp"

namespace tactile::cmd {

MoveAnimationFrameForwards::MoveAnimationFrameForwards(const Entity tile_entity,
                                                       const usize frame_index)
    : mTileEntity {tile_entity},
      mFrameIndex {frame_index}
{
}

void MoveAnimationFrameForwards::undo()
{
  auto& model = get_global_model();
  auto& animation = model.get<TileAnimation>(mTileEntity);

  const TileAnimationFrame frame {
      .tile_index = animation.frames.at(mFrameIndex).tile_index,
      .duration = animation.frames.at(mFrameIndex).duration,
  };

  erase_at(animation.frames, mFrameIndex - 1);
  insert_at(animation.frames, mFrameIndex, frame);
}

void MoveAnimationFrameForwards::redo()
{
  auto& model = get_global_model();
  auto& animation = model.get<TileAnimation>(mTileEntity);

  const TileAnimationFrame frame {
      .tile_index = animation.frames.at(mFrameIndex).tile_index,
      .duration = animation.frames.at(mFrameIndex).duration,
  };

  erase_at(animation.frames, mFrameIndex);
  insert_at(animation.frames, mFrameIndex - 1, frame);
}

auto MoveAnimationFrameForwards::get_name() const -> String
{
  const auto& strings = sys::get_current_language_strings(get_global_model());
  return strings.cmd.move_animation_frame_forwards;
}

}  // namespace tactile::cmd