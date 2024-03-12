// Copyright (C) 2024 Albin Johansson (GNU General Public License v3.0)

#pragma once

#include "core/tile/tile_animation.hpp"
#include "model/cmd/command.hpp"
#include "tactile/base/container/maybe.hpp"
#include "tactile/base/int.hpp"
#include "tactile/base/prelude.hpp"

TACTILE_FWD_DECLARE_CLASS_NS(tactile, TilesetDocument)

namespace tactile::cmd {

class DeleteAnimation final : public Command {
 public:
  DeleteAnimation(TilesetDocument* document, TileIndex tile_index);

  void undo() override;

  void redo() override;

  [[nodiscard]] auto get_name() const -> String override;

 private:
  TilesetDocument* mDocument {};
  TileIndex mTileIndex {};
  Maybe<TileAnimation> mRemovedAnimation;
};

}  // namespace tactile::cmd
