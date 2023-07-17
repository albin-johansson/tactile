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
#include "model/model.hpp"

namespace tactile {

void on_center_viewport(Model& model,
                        Dispatcher& dispatcher,
                        const CenterViewportEvent& event);

void on_center_viewport(Model& model, const CenterViewportEvent& event);

void on_reset_viewport_zoom(Model& model, const ResetViewportZoomEvent& event);

void on_increase_viewport_zoom(Model& model, const IncreaseViewportZoomEvent& event);

void on_decrease_viewport_zoom(Model& model, const DecreaseViewportZoomEvent& event);

void on_offset_viewport(Model& model, const OffsetViewportEvent& event);

void on_set_viewport_limits(Model& model, const SetViewportLimitsEvent& event);

void on_set_dynamic_viewport_info(Model& model, const SetDynamicViewportInfoEvent& event);

void on_pan_viewport_up(Model& model, const PanViewportUpEvent& event);

void on_pan_viewport_down(Model& model, const PanViewportDownEvent& event);

void on_pan_viewport_left(Model& model, const PanViewportLeftEvent& event);

void on_pan_viewport_right(Model& model, const PanViewportRightEvent& event);

}  // namespace tactile
