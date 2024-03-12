// Copyright (C) 2024 Albin Johansson (GNU General Public License v3.0)

#pragma once

#include "model/cmd/command.hpp"
#include "tactile/base/container/maybe.hpp"
#include "tactile/base/id.hpp"
#include "tactile/base/int.hpp"
#include "tactile/base/prelude.hpp"

TACTILE_FWD_DECLARE_CLASS_NS(tactile, TilesetDocument)

namespace tactile::cmd {

class MoveAnimationFrameForwards final : public Command {
 public:
  MoveAnimationFrameForwards(TilesetDocument* document,
                             TileIndex tile_index,
                             usize frame_index);

  void undo() override;

  void redo() override;

  [[nodiscard]] auto get_name() const -> String override;

 private:
  TilesetDocument* mDocument {};
  TileIndex mTileIndex {};
  usize mFrameIndex {};
};

}  // namespace tactile::cmd
