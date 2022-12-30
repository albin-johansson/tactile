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

#include "add_animation_frame.hpp"

#include "core/debug/panic.hpp"
#include "core/tile/tile.hpp"
#include "lang/language.hpp"
#include "lang/strings.hpp"
#include "model/document/tileset_document.hpp"

namespace tactile::cmd {

AddAnimationFrame::AddAnimationFrame(TilesetDocument* document,
                                     const TileIndex animated_tile_index,
                                     const TileIndex frame_tile_index,
                                     const ms_t frame_duration)
    : mDocument {document},
      mAnimatedTileIndex {animated_tile_index},
      mFrameTileIndex {frame_tile_index},
      mFrameDuration {frame_duration}
{
  if (!mDocument) {
    throw TactileError {"Invalid null tileset document"};
  }
}

void AddAnimationFrame::undo()
{
  auto& tileset = mDocument->get_tileset();
  auto& tile = tileset[mAnimatedTileIndex];

  auto& animation = tile.get_animation();
  animation.remove_frame(mFrameIndex.value());

  if (!mTileWasAnimatedBefore) {
    tile.clear_animation();
  }

  mFrameIndex.reset();
}

void AddAnimationFrame::redo()
{
  auto& tileset = mDocument->get_tileset();
  auto& tile = tileset[mAnimatedTileIndex];

  mTileWasAnimatedBefore = tile.is_animated();

  if (!tile.is_animated()) {
    TileAnimation animation;
    animation.add_frame(mAnimatedTileIndex, ms_t {1'000});
    tile.set_animation(animation);
  }

  auto& animation = tile.get_animation();
  animation.add_frame(mFrameTileIndex, mFrameDuration);

  mFrameIndex = animation.size() - 1;
}

auto AddAnimationFrame::get_name() const -> String
{
  const auto& lang = get_current_language();
  return lang.cmd.add_animation_frame;
}

}  // namespace tactile::cmd