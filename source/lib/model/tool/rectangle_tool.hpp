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

#include "common/type/math.hpp"
#include "common/type/maybe.hpp"
#include "model/tool/tool.hpp"

namespace tactile {

struct CurrentRectangleStroke final {
  Float2 start {};
  Float2 current {};
};

class RectangleTool final : public Tool {
 public:
  void accept(ToolVisitor& visitor) const override;

  void on_disabled(Model& model, Dispatcher& dispatcher) override;

  void on_exited(Model& model, Dispatcher& dispatcher) override;

  void on_pressed(Model& model, Dispatcher& dispatcher, const MouseInfo& mouse) override;

  void on_dragged(Model& model, Dispatcher& dispatcher, const MouseInfo& mouse) override;

  void on_released(Model& model, Dispatcher& dispatcher, const MouseInfo& mouse) override;

  [[nodiscard]] auto is_available(const Model& model) const -> bool override;

  [[nodiscard]] auto get_stroke() const -> const Maybe<CurrentRectangleStroke>&;

  [[nodiscard]] auto get_type() const -> ToolType override { return ToolType::Rectangle; }

 private:
  Maybe<CurrentRectangleStroke> mStroke;

  void maybe_emit_event(Model& model, Dispatcher& dispatcher);
};

}  // namespace tactile
