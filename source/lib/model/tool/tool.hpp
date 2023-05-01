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

#include "common/type/dispatcher.hpp"
#include "model/model.hpp"
#include "model/mouse_info.hpp"
#include "model/tool/tool_type.hpp"
#include "model/tool/tool_visitor.hpp"

namespace tactile {

class Tool {
 public:
  virtual ~Tool() noexcept = default;

  virtual void accept(ToolVisitor& visitor) const = 0;

  virtual void on_enabled(Model& model, Dispatcher& dispatcher);

  virtual void on_disabled(Model& model, Dispatcher& dispatcher);

  virtual void on_entered(Model& model, Dispatcher& dispatcher);

  virtual void on_exited(Model& model, Dispatcher& dispatcher);

  virtual void on_pressed(Model& model, Dispatcher& dispatcher, const MouseInfo& mouse);

  virtual void on_dragged(Model& model, Dispatcher& dispatcher, const MouseInfo& mouse);

  virtual void on_released(Model& model, Dispatcher& dispatcher, const MouseInfo& mouse);

  [[nodiscard]] virtual auto is_available(const Model& model) const -> bool = 0;

  [[nodiscard]] virtual auto get_type() const -> ToolType = 0;
};

}  // namespace tactile
