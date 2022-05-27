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

#include "eraser_tool.hpp"

#include <utility>  // move

#include <centurion/mouse.hpp>
#include <entt/signal/dispatcher.hpp>

#include "core/common/ecs.hpp"
#include "core/components/layers.hpp"
#include "core/mouse_info.hpp"
#include "core/systems/layers/layer_system.hpp"
#include "core/systems/layers/tile_layer_system.hpp"
#include "core/common/ecs.hpp"
#include "core/events/tool_events.hpp"

namespace tactile {

void EraserTool::on_disabled(entt::registry&, entt::dispatcher& dispatcher)
{
  maybe_emit_event(dispatcher);
}

void EraserTool::on_exited(entt::registry&, entt::dispatcher& dispatcher)
{
  maybe_emit_event(dispatcher);
}

void EraserTool::on_pressed(entt::registry& registry,
                            entt::dispatcher&,
                            const MouseInfo& mouse)
{
  if (mouse.is_within_contents && mouse.button == cen::mouse_button::left &&
      sys::is_tile_layer_active(registry)) {
    mPrevState.clear();
    update_sequence(registry, mouse.position_in_viewport);
  }
}

void EraserTool::on_dragged(entt::registry& registry,
                            entt::dispatcher&,
                            const MouseInfo& mouse)
{
  if (mouse.is_within_contents && mouse.button == cen::mouse_button::left &&
      sys::is_tile_layer_active(registry)) {
    update_sequence(registry, mouse.position_in_viewport);
  }
}

void EraserTool::on_released(entt::registry& registry,
                             entt::dispatcher& dispatcher,
                             const MouseInfo& mouse)
{
  if (mouse.button == cen::mouse_button::left && sys::is_tile_layer_active(registry)) {
    maybe_emit_event(dispatcher);
  }
}

auto EraserTool::is_available(const entt::registry& registry) const -> bool
{
  return sys::is_tile_layer_active(registry);
}

auto EraserTool::get_type() const -> ToolType
{
  return ToolType::Eraser;
}

void EraserTool::update_sequence(entt::registry& registry, const TilePos& cursor)
{
  const auto entity = sys::get_active_layer(registry);
  auto& layer = checked_get<comp::TileLayer>(registry, entity);

  if (!mPrevState.contains(cursor)) {
    mPrevState.emplace(cursor, sys::get_tile(layer, cursor));
  }

  sys::set_tile(layer, cursor, empty_tile);
}

void EraserTool::maybe_emit_event(entt::dispatcher& dispatcher)
{
  if (!mPrevState.empty()) {
    dispatcher.enqueue<EraserSequenceEvent>(std::move(mPrevState));
    mPrevState.clear();
  }
}

}  // namespace tactile