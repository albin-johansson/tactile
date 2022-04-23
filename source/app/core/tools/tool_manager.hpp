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

#include <memory>  // unique_ptr

#include "core/tools/tool.hpp"
#include "core/tools/tool_type.hpp"
#include "misc/macros.hpp"
#include "tactile.hpp"

namespace tactile {

class ToolManager final : public ATool
{
 public:
  TACTILE_DELETE_COPY(ToolManager);
  TACTILE_DECLARE_MOVE(ToolManager);

  ToolManager();

  ~ToolManager() noexcept override;

  void select_tool(ToolType type, entt::registry& registry, entt::dispatcher& dispatcher);

  [[nodiscard]] auto is_enabled(ToolType type) const -> bool;

  void on_enabled(entt::registry& registry, entt::dispatcher& dispatcher) override;

  void on_disabled(entt::registry& registry, entt::dispatcher& dispatcher) override;

  void on_entered(entt::registry& registry, entt::dispatcher& dispatcher) override;

  void on_exited(entt::registry& registry, entt::dispatcher& dispatcher) override;

  void on_pressed(entt::registry& registry,
                  entt::dispatcher& dispatcher,
                  const MouseInfo& mouse) override;

  void on_dragged(entt::registry& registry,
                  entt::dispatcher& dispatcher,
                  const MouseInfo& mouse) override;

  void on_released(entt::registry& registry,
                   entt::dispatcher& dispatcher,
                   const MouseInfo& mouse) override;

  /* Do not call this function, it will just throw an exception */
  [[nodiscard, deprecated]] auto get_type() const -> ToolType override;

 private:
  struct Data;
  std::unique_ptr<Data> mData;
};

}  // namespace tactile
