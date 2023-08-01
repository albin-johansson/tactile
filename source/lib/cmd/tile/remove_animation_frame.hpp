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

#include <chrono>  // milliseconds

#include "cmd/command.hpp"
#include "core/ecs/entity.hpp"
#include "core/functional/maybe.hpp"
#include "core/prelude.hpp"
#include "model/registry.hpp"
#include "model/tiles/tile_components.hpp"

namespace tactile::cmd {

class RemoveAnimationFrame final : public Command {
 public:
  using ms_type = std::chrono::milliseconds;

  RemoveAnimationFrame(Registry* registry, Entity tile_entity, usize frame_index);

  void undo() override;

  void redo() override;

  [[nodiscard]] auto get_name() const -> String override;

 private:
  Registry* mRegistry;
  Entity mTileEntity;
  usize mFrameIndex;
  Maybe<TileIndex> mFrameTileIndex;
  Maybe<ms_type> mFrameDuration;
  Maybe<TileAnimation> mRemovedAnimation;  // Only set when removing the last frame
};

}  // namespace tactile::cmd
