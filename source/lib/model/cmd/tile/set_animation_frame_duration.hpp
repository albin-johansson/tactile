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

class SetAnimationFrameDuration final : public Command {
 public:
  SetAnimationFrameDuration(TilesetDocument* document,
                            TileIndex tile_index,
                            usize frame_index,
                            Milliseconds frame_duration);

  void undo() override;

  void redo() override;

  [[nodiscard]] auto merge_with(const Command* cmd) -> bool override;

  [[nodiscard]] auto get_name() const -> String override;

 private:
  TilesetDocument* mDocument {};
  TileIndex mTileIndex {};
  usize mFrameIndex {};
  Milliseconds mNewFrameDuration {};
  Maybe<Milliseconds> mOldFrameDuration {};
};

}  // namespace tactile::cmd
