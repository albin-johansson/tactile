/*
 * This source file is a part of the Tactile map editor.
 *
 * Copyright (C) 2022 Albin Johansson
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

#include "core/tile_cache.hpp"
#include "editor/commands/command.hpp"
#include "editor/commands/command_id.hpp"
#include "tactile.hpp"

namespace tactile {

/// \addtogroup commands
/// \{

class EraserToolCmd final : public ACommand
{
 public:
  EraserToolCmd(registry_ref registry, TileCache&& oldState);

  void undo() override;

  void redo() override;

  [[nodiscard]] auto id() const noexcept -> int override
  {
    return CommandId::eraser_sequence;
  }

 private:
  registry_ref mRegistry;
  layer_id mLayer;
  TileCache mOldState;
};

/// \} End of group commands

}  // namespace tactile