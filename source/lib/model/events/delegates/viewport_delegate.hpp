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
#include "model/events/viewport_events.hpp"
#include "model/registry.hpp"

namespace tactile {

void on_center_viewport(Registry& registry,
                        Dispatcher& dispatcher,
                        const CenterViewportEvent& event);

void on_center_viewport(Registry& registry, const CenterViewportEvent& event);

void on_reset_viewport_zoom(Registry& registry, const ResetViewportZoomEvent& event);

void on_increase_viewport_zoom(Registry& registry,
                               const IncreaseViewportZoomEvent& event);

void on_decrease_viewport_zoom(Registry& registry,
                               const DecreaseViewportZoomEvent& event);

void on_offset_viewport(Registry& registry, const OffsetViewportEvent& event);

void on_set_viewport_limits(Registry& registry, const SetViewportLimitsEvent& event);

void on_set_dynamic_viewport_info(Registry& registry,
                                  const SetDynamicViewportInfoEvent& event);

void on_pan_viewport_up(Registry& registry, const PanViewportUpEvent& event);

void on_pan_viewport_down(Registry& registry, const PanViewportDownEvent& event);

void on_pan_viewport_left(Registry& registry, const PanViewportLeftEvent& event);

void on_pan_viewport_right(Registry& registry, const PanViewportRightEvent& event);

}  // namespace tactile
