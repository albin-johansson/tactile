// Copyright (C) 2024 Albin Johansson (GNU General Public License v3.0)

#include "set_animation_frame_duration.hpp"

#include "core/tile/tile.hpp"
#include "lang/language.hpp"
#include "lang/strings.hpp"
#include "model/document/tileset_document.hpp"
#include "tactile/core/debug/exception.hpp"

namespace tactile::cmd {

SetAnimationFrameDuration::SetAnimationFrameDuration(TilesetDocument* document,
                                                     const TileIndex tile_index,
                                                     const usize frame_index,
                                                     const Milliseconds frame_duration)
    : mDocument {document},
      mTileIndex {tile_index},
      mFrameIndex {frame_index},
      mNewFrameDuration {frame_duration}
{
  if (!mDocument) {
    throw Exception {"Invalid null tileset document"};
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
