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
#include "core/tools/tool.hpp"

namespace tactile {

/**
 * \brief A tool used to draw (or "stamp") selected tileset tiles to tile layers.
 *
 * \todo Right-click should erase tiles.
 */
class StampTool final : public ATool
{
 public:
  void on_disabled(entt::registry& registry, entt::dispatcher& dispatcher) override;

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

  [[nodiscard]] auto get_type() const -> ToolType override;

 private:
  TileCache mPrevious;  ///< Previous tile state.
  TileCache mCurrent;   ///< The current stamp sequence.

  void update_sequence(entt::registry& registry, const TilePos& cursor);

  void maybe_emit_event(entt::dispatcher& dispatcher);

  [[nodiscard]] auto is_usable(const entt::registry& registry) const -> bool;
};

}  // namespace tactile
