// Copyright (C) 2024 Albin Johansson (GNU General Public License v3.0)

#include "remove_animation_frame.hpp"

#include "core/tile/tile.hpp"
#include "lang/language.hpp"
#include "lang/strings.hpp"
#include "model/document/tileset_document.hpp"
#include "tactile/core/debug/exception.hpp"

namespace tactile::cmd {

RemoveAnimationFrame::RemoveAnimationFrame(TilesetDocument* document,
                                           const TileIndex tile_index,
                                           const usize frame_index)
    : mDocument {document},
      mTileIndex {tile_index},
      mFrameIndex {frame_index}
{
  if (!mDocument) {
    throw Exception {"Invalid null tileset document"};
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
