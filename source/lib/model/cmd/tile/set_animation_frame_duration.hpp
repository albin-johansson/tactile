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

#pragma once

#include "common/numeric.hpp"
#include "common/type/chrono.hpp"
#include "common/type/maybe.hpp"
#include "model/cmd/command.hpp"
#include "tactile/base/prelude.hpp"

TACTILE_FWD_DECLARE_CLASS_NS(tactile, TilesetDocument)

namespace tactile::cmd {

class SetAnimationFrameDuration final : public Command {
 public:
  SetAnimationFrameDuration(TilesetDocument* document,
                            TileIndex tile_index,
                            usize frame_index,
                            ms_t frame_duration);

  void undo() override;

  void redo() override;

  [[nodiscard]] auto merge_with(const Command* cmd) -> bool override;

  [[nodiscard]] auto get_name() const -> String override;

 private:
  TilesetDocument* mDocument {};
  TileIndex mTileIndex {};
  usize mFrameIndex {};
  ms_t mNewFrameDuration {};
  Maybe<ms_t> mOldFrameDuration {};
};

}  // namespace tactile::cmd
