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

#include "core/commands/command.hpp"
#include "core/commands/command_id.hpp"
#include "core/common/maybe.hpp"
#include "core/common/uuid.hpp"
#include "core/fwd.hpp"
#include "core/systems/snapshot.hpp"

namespace tactile {

/// \addtogroup commands
/// \{

class AddLayerCmd final : public ACommand
{
 public:
  AddLayerCmd(MapDocument* map, LayerType type);

  void undo() override;

  void redo() override;

  [[nodiscard]] auto id() const noexcept -> int override { return CommandId::add_layer; }

 private:
  MapDocument* mMap{};
  LayerType mLayerType;
  Maybe<UUID> mLayerId;
  Maybe<sys::LayerSnapshot> mLayerSnapshot;
};

/// \} End of group commands

}  // namespace tactile
