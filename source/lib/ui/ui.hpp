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

#include <entt/fwd.hpp>

#include "io/map/parse/parse_error.hpp"
#include "tactile/core/prelude.hpp"

TACTILE_FWD_NS(tactile, class DocumentModel)

namespace tactile::ui {

/// Updates the state of the GUI.
void update_widgets(const DocumentModel& model, entt::dispatcher& dispatcher);

void open_map_file_dialog();

void open_about_dear_imgui_dialog();

[[nodiscard]] auto is_editor_focused() -> bool;

}  // namespace tactile::ui