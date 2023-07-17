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

#include "common/debug/assert.hpp"
#include "common/util/vectors.hpp"
#include "model/entity_validation.hpp"
#include "model/i18n/language_system.hpp"
#include "model/tiles/tile_animation_system.hpp"
#include "model/tiles/tile_components.hpp"
#include "model/tilesets/tileset_system.hpp"

namespace tactile::cmd {

AddAnimationFrame::AddAnimationFrame(Registry* registry,
                                     const Entity tile_entity,
                                     const TileIndex frame_tile_index,
                                     const ms_t frame_duration)
    : mRegistry {registry},
      mTileEntity {tile_entity},
      mFrameTileIndex {frame_tile_index},
      mFrameDuration {frame_duration}
{
  TACTILE_ASSERT(sys::is_tile_entity(*mRegistry, tile_entity));
}

void AddAnimationFrame::undo()
{
  auto& registry = *mRegistry;

  auto& animation = registry.get<TileAnimation>(mTileEntity);
  erase_at(animation.frames, mFrameIndex.value());

  if (!mTileWasAnimatedBefore) {
    registry.remove<TileAnimation>(mTileEntity);
  }

  mFrameIndex.reset();
}

void AddAnimationFrame::redo()
{
  auto& registry = *mRegistry;
  mTileWasAnimatedBefore = registry.has<TileAnimation>(mTileEntity);

  if (!mTileWasAnimatedBefore) {
    sys::make_tile_animated(registry, mTileEntity);
  }

  auto& animation = registry.get<TileAnimation>(mTileEntity);

  auto& frame = animation.frames.emplace_back();
  frame.tile_index = mFrameTileIndex;
  frame.duration = mFrameDuration;

  mFrameIndex = animation.frames.size() - 1;
}

auto AddAnimationFrame::get_name() const -> String
{
  const auto& strings = sys::get_current_language_strings(*mRegistry);
  return strings.cmd.add_animation_frame;
}

}  // namespace tactile::cmd