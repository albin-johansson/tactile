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

#include "core/common/tile_cache.hpp"
#include "core/fwd.hpp"
#include "core/tool/tool.hpp"

namespace tactile {

class EraserTool final : public ATool
{
 public:
  void on_disabled(DocumentModel& model, entt::dispatcher& dispatcher) override;

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

  [[nodiscard]] auto is_available(const DocumentModel& model) const -> bool override;

  [[nodiscard]] auto get_type() const -> ToolType override { return ToolType::Eraser; }

 private:
  TileCache mPrevState;

  void update_sequence(DocumentModel& model, const TilePos& cursor);

  void maybe_emit_event(const DocumentModel& model, entt::dispatcher& dispatcher);
};

}  // namespace tactile
