// Copyright (C) 2024 Albin Johansson (GNU General Public License v3.0)

#include "add_animation_frame.hpp"

#include "core/tile/tile.hpp"
#include "lang/language.hpp"
#include "lang/strings.hpp"
#include "model/document/tileset_document.hpp"
#include "tactile/core/debug/exception.hpp"

namespace tactile::cmd {

AddAnimationFrame::AddAnimationFrame(TilesetDocument* document,
                                     const TileIndex animated_tile_index,
                                     const TileIndex frame_tile_index,
                                     const Milliseconds frame_duration)
    : mDocument {document},
      mAnimatedTileIndex {animated_tile_index},
      mFrameTileIndex {frame_tile_index},
      mFrameDuration {frame_duration}
{
  if (!mDocument) {
    throw Exception {"Invalid null tileset document"};
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
    animation.add_frame(mAnimatedTileIndex, Milliseconds {1'000});
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
