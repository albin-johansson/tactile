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

#include "core/common/macros.hpp"
#include "core/common/memory.hpp"
#include "core/tool/tool.hpp"
#include "core/tool/tool_type.hpp"
#include "core/tool/tool_visitor.hpp"

namespace tactile {

/**
 * Manages the tools associated with a map document.
 */
class ToolManager final : private ATool
{
 public:
  TACTILE_DELETE_COPY(ToolManager);
  TACTILE_DECLARE_MOVE(ToolManager);

  ToolManager();

  ~ToolManager() noexcept override;

  void accept(IToolVisitor& visitor) const override;

  void select_tool(ToolType type, DocumentModel& model, entt::dispatcher& dispatcher);

  void set_stamp_random_mode(bool random);

  [[nodiscard]] auto is_enabled(ToolType type) const -> bool;

  [[nodiscard]] auto is_stamp_random() const -> bool;

  [[nodiscard]] auto is_available(const DocumentModel& model, ToolType type) const
      -> bool;

  void on_enabled(DocumentModel& model, entt::dispatcher& dispatcher) override;

  void on_disabled(DocumentModel& model, entt::dispatcher& dispatcher) override;

  void on_entered(DocumentModel& model, entt::dispatcher& dispatcher) override;

  void on_exited(DocumentModel& model, entt::dispatcher& dispatcher) override;

  void on_pressed(DocumentModel&    model,
                  entt::dispatcher& dispatcher,
                  const MouseInfo&  mouse) override;

  void on_dragged(DocumentModel&    model,
                  entt::dispatcher& dispatcher,
                  const MouseInfo&  mouse) override;

  void on_released(DocumentModel&    model,
                   entt::dispatcher& dispatcher,
                   const MouseInfo&  mouse) override;

 private:
  struct Data;
  Unique<Data> mData;

  /* Do not call these functions, they will just raise exceptions */
  [[nodiscard, deprecated]] auto get_type() const -> ToolType override;
  [[nodiscard, deprecated]] auto is_available(const DocumentModel& model) const
      -> bool override;
};

}  // namespace tactile
