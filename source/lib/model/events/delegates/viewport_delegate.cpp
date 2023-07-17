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

#include "viewport_delegate.hpp"

#include <cmath>  // round

#include "model/documents/document_components.hpp"
#include "model/documents/document_system.hpp"
#include "model/viewports/viewport_ops.hpp"
#include "model/viewports/viewport_system.hpp"

namespace tactile {

void on_center_viewport(Registry& registry, const CenterViewportEvent& event)
{
  auto& viewport = registry.get<Viewport>(event.viewport);
  const auto& viewport_info = registry.get<DynamicViewportInfo>(event.viewport);

  if (viewport_info.total_size.has_value() && viewport_info.content_size.has_value()) {
    const auto total_size = *viewport_info.total_size;
    const auto content_size = *viewport_info.content_size;

    const auto raw_offset = ((total_size - content_size) / 2.0f) - viewport.offset;
    const Float2 rounded_offset {std::round(raw_offset.x), std::round(raw_offset.y)};

    on_offset_viewport(registry, OffsetViewportEvent {event.viewport, rounded_offset});
  }
}

void on_reset_viewport_zoom(Registry& registry, const ResetViewportZoomEvent& event)
{
  auto& viewport = registry.get<Viewport>(event.viewport);
  sys::reset_viewport_zoom(viewport);
}

void on_increase_viewport_zoom(Registry& registry, const IncreaseViewportZoomEvent& event)
{
  auto& viewport = registry.get<Viewport>(event.viewport);
  sys::increase_viewport_zoom(viewport, event.anchor_pos);
}

void on_decrease_viewport_zoom(Registry& registry, const DecreaseViewportZoomEvent& event)
{
  auto& viewport = registry.get<Viewport>(event.viewport);
  sys::decrease_viewport_zoom(viewport, event.anchor_pos);
}

void on_offset_viewport(Registry& registry, const OffsetViewportEvent& event)
{
  auto& viewport = registry.get<Viewport>(event.viewport);
  sys::offset_viewport(viewport, event.delta);
}

void on_set_viewport_limits(Registry& registry, const SetViewportLimitsEvent& event)
{
  auto& viewport = registry.get<Viewport>(event.viewport);
  viewport.limits = ViewportLimits {event.min_offset, event.max_offset};
}

void on_set_dynamic_viewport_info(Registry& registry,
                                  const SetDynamicViewportInfoEvent& event)
{
  auto& dynamic_info = registry.get<DynamicViewportInfo>(event.viewport);
  dynamic_info = event.info;
}

void on_pan_viewport_up(Registry& registry, const PanViewportUpEvent& event)
{
  auto& viewport = registry.get<Viewport>(event.viewport);
  viewport.offset.y += viewport.tile_size.y;
}

void on_pan_viewport_down(Registry& registry, const PanViewportDownEvent& event)
{
  auto& viewport = registry.get<Viewport>(event.viewport);
  viewport.offset.y -= viewport.tile_size.y;
}

void on_pan_viewport_left(Registry& registry, const PanViewportLeftEvent& event)
{
  auto& viewport = registry.get<Viewport>(event.viewport);
  viewport.offset.x += viewport.tile_size.x;
}

void on_pan_viewport_right(Registry& registry, const PanViewportRightEvent& event)
{
  auto& viewport = registry.get<Viewport>(event.viewport);
  viewport.offset.x -= viewport.tile_size.x;
}

}  // namespace tactile
