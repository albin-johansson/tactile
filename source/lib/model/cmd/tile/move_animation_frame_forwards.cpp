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

#include "common/debug/assert.hpp"
#include "core/tile/tile.hpp"
#include "lang/language.hpp"
#include "lang/strings.hpp"
#include "model/document/tileset_document.hpp"
#include "tactile/core/debug/error.hpp"

namespace tactile::cmd {

MoveAnimationFrameForwards::MoveAnimationFrameForwards(TilesetDocument* document,
                                                       const TileIndex tile_index,
                                                       const usize frame_index)
    : mDocument {document},
      mTileIndex {tile_index},
      mFrameIndex {frame_index}
{
  if (!mDocument) {
    throw Error {"Invalid null tileset document"};
  }
}

void MoveAnimationFrameForwards::undo()
{
  TACTILE_ASSERT(mFrameIndex - 1 >= 0);

  auto& tileset = mDocument->get_tileset();
  auto& tile = tileset[mTileIndex];
  auto& animation = tile.get_animation();

  const auto frame_tile_index = animation[mFrameIndex - 1].tile;
  const auto frame_duration = animation[mFrameIndex - 1].duration;

  animation.remove_frame(mFrameIndex - 1);
  animation.insert_frame(mFrameIndex, frame_tile_index, frame_duration);
}

void MoveAnimationFrameForwards::redo()
{
  TACTILE_ASSERT(mFrameIndex - 1 >= 0);

  auto& tileset = mDocument->get_tileset();
  auto& tile = tileset[mTileIndex];
  auto& animation = tile.get_animation();

  const auto frame_tile_index = animation[mFrameIndex].tile;
  const auto frame_duration = animation[mFrameIndex].duration;

  animation.remove_frame(mFrameIndex);
  animation.insert_frame(mFrameIndex - 1, frame_tile_index, frame_duration);
}

auto MoveAnimationFrameForwards::get_name() const -> String
{
  const auto& lang = get_current_language();
  return lang.cmd.move_animation_frame_forwards;
}

}  // namespace tactile::cmd