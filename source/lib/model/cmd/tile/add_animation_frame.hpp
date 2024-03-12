// Copyright (C) 2024 Albin Johansson (GNU General Public License v3.0)

#pragma once

#include "model/cmd/command.hpp"
#include "tactile/base/container/maybe.hpp"
#include "tactile/base/id.hpp"
#include "tactile/base/int.hpp"
#include "tactile/base/prelude.hpp"
#include "tactile/base/util/chrono.hpp"

TACTILE_FWD_DECLARE_CLASS_NS(tactile, TilesetDocument)

namespace tactile::cmd {

class AddAnimationFrame final : public Command {
 public:
  AddAnimationFrame(TilesetDocument* document,
                    TileIndex animated_tile_index,
                    TileIndex frame_tile_index,
                    Milliseconds frame_duration);

  void undo() override;

  void redo() override;

  [[nodiscard]] auto get_name() const -> String override;

 private:
  TilesetDocument* mDocument {};
  TileIndex mAnimatedTileIndex {};
  TileIndex mFrameTileIndex {};
  Milliseconds mFrameDuration {};
  Maybe<usize> mFrameIndex;
  bool mTileWasAnimatedBefore : 1 {};
};

}  // namespace tactile::cmd
