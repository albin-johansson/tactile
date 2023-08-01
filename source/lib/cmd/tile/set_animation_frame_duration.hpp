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

#include "cmd/command.hpp"
#include "common/type/chrono.hpp"
#include "common/type/ecs.hpp"
#include "core/functional/maybe.hpp"
#include "core/prelude.hpp"
#include "model/registry.hpp"

namespace tactile::cmd {

class SetAnimationFrameDuration final : public Command {
 public:
  SetAnimationFrameDuration(Registry* registry,
                            Entity tile_entity,
                            usize frame_index,
                            ms_t new_frame_duration);

  void undo() override;

  void redo() override;

  [[nodiscard]] auto merge_with(const Command* cmd) -> bool override;

  [[nodiscard]] auto get_name() const -> String override;

 private:
  Registry* mRegistry;
  Entity mTileEntity;
  usize mFrameIndex;
  ms_t mNewFrameDuration;
  Maybe<ms_t> mOldFrameDuration {};
};

}  // namespace tactile::cmd
