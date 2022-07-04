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
#include "core/commands/maps/map_command_cache.hpp"
#include "core/common/ints.hpp"
#include "core/fwd.hpp"

namespace tactile {

class RemoveColumnCmd final : public ICommand
{
 public:
  explicit RemoveColumnCmd(MapDocument* document);

  void undo() override;

  void redo() override;

  [[nodiscard]] auto merge_with(const ICommand& cmd) -> bool override;

  [[nodiscard]] auto id() const noexcept -> CommandId override
  {
    return CommandId::MapRemoveColumn;
  }

  [[nodiscard]] auto get_name() const -> const char* override;

 private:
  MapDocument*    mDocument{};
  MapCommandCache mCache;
  usize           mColumns{1};
};

}  // namespace tactile
