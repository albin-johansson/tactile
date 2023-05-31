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
#include "common/type/ecs.hpp"
#include "components/viewport.hpp"
#include "model/model.hpp"

namespace tactile::sys {

auto create_stamp_tool(Model& model) -> Entity;

void on_stamp_tool_deactivated(Model& model, Entity tool_entity, Dispatcher& dispatcher);

void on_stamp_tool_exited(Model& model, Entity tool_entity, Dispatcher& dispatcher);

void on_stamp_tool_pressed(Model& model,
                           Entity tool_entity,
                           const ViewportMouseInfo& mouse_info,
                           Dispatcher& dispatcher);

void on_stamp_tool_dragged(Model& model,
                           Entity tool_entity,
                           const ViewportMouseInfo& mouse_info,
                           Dispatcher& dispatcher);

void on_stamp_tool_released(Model& model,
                            Entity tool_entity,
                            const ViewportMouseInfo& mouse_info,
                            Dispatcher& dispatcher);

[[nodiscard]] auto is_stamp_tool_available(const Model& model) -> bool;

}  // namespace tactile::sys