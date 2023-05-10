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

#include "add_animation_frame.hpp"

#include "common/util/vector.hpp"
#include "core/tile.hpp"
#include "lang/language.hpp"
#include "lang/strings.hpp"
#include "model/model.hpp"
#include "model/systems/tileset_system.hpp"

namespace tactile::cmd {

AddAnimationFrame::AddAnimationFrame(const Entity tile_entity,
                                     const TileIndex frame_tile_index,
                                     const ms_t frame_duration)
    : mTileEntity {tile_entity},
      mFrameTileIndex {frame_tile_index},
      mFrameDuration {frame_duration}
{
}

void AddAnimationFrame::undo()
{
  auto& model = get_model();

  auto& animation = model.get<TileAnimation>(mTileEntity);
  erase_at(animation.frames, mFrameIndex.value());

  if (!mTileWasAnimatedBefore) {
    model.remove<TileAnimation>(mTileEntity);
  }

  mFrameIndex.reset();
}

void AddAnimationFrame::redo()
{
  auto& model = get_model();
  mTileWasAnimatedBefore = model.has<TileAnimation>(mTileEntity);

  if (!mTileWasAnimatedBefore) {
    sys::make_tile_animated(model, mTileEntity);
  }

  auto& animation = model.get<TileAnimation>(mTileEntity);

  auto& frame = animation.frames.emplace_back();
  frame.tile_index = mFrameTileIndex;
  frame.duration = mFrameDuration;

  mFrameIndex = animation.frames.size() - 1;
}

auto AddAnimationFrame::get_name() const -> String
{
  const auto& lang = get_current_language();
  return lang.cmd.add_animation_frame;
}

}  // namespace tactile::cmd