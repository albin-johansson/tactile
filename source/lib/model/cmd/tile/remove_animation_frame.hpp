// Copyright (C) 2024 Albin Johansson (GNU General Public License v3.0)

#pragma once

#include "core/tile/tile_animation.hpp"
#include "model/cmd/command.hpp"
#include "tactile/base/container/maybe.hpp"
#include "tactile/base/int.hpp"
#include "tactile/base/prelude.hpp"
#include "tactile/base/util/chrono.hpp"

TACTILE_FWD_DECLARE_CLASS_NS(tactile, TilesetDocument)

namespace tactile::cmd {

class RemoveAnimationFrame final : public Command {
 public:
  RemoveAnimationFrame(TilesetDocument* document,
                       TileIndex tile_index,
                       usize frame_index);

  void undo() override;

  void redo() override;

  [[nodiscard]] auto get_name() const -> String override;

 private:
  TilesetDocument* mDocument {};
  TileIndex mTileIndex {};
  usize mFrameIndex {};
  Maybe<TileIndex> mFrameTileIndex;
  Maybe<Milliseconds> mFrameDuration;
  Maybe<TileAnimation> mRemovedAnimation;  // Only set when removing the last frame
};

}  // namespace tactile::cmd
