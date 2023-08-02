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

#include "core/containers/smart_ptr.hpp"
#include "core/containers/vector.hpp"
#include "core/prelude.hpp"
#include "model/system.hpp"

namespace tactile {

TACTILE_FWD_DECLARE_CLASS(CommandSystem)
TACTILE_FWD_DECLARE_CLASS(ToolSystem)

class SystemManager final : private System {
 public:
  SystemManager();

  ~SystemManager() noexcept override;

  void reset() override;

  void update(Registry& registry, Dispatcher& dispatcher) override;

  [[nodiscard]] auto get_command_system() -> CommandSystem&;
  [[nodiscard]] auto get_command_system() const -> const CommandSystem&;

  [[nodiscard]] auto get_tool_system() -> ToolSystem&;
  [[nodiscard]] auto get_tool_system() const -> const ToolSystem&;

 private:
  Unique<CommandSystem> mCommandSystem;
  Unique<ToolSystem> mToolSystem;
  Vector<System*> mSystems;
};

}  // namespace tactile
