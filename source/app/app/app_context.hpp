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

#include <centurion/window.hpp>
#include <entt/fwd.hpp>

#include "core/vocabulary.hpp"

namespace tactile {

TACTILE_FWD_DECLARE_CLASS(AppCfg)
TACTILE_FWD_DECLARE_CLASS(DocumentModel)
TACTILE_FWD_DECLARE_STRUCT(WidgetShowState)

void init_app_context(cen::window& window);

void request_font_reload();
void handled_font_reload();

[[nodiscard]] auto get_window() -> cen::window&;
[[nodiscard]] auto get_dispatcher() -> entt::dispatcher&;
[[nodiscard]] auto get_model() -> DocumentModel&;
[[nodiscard]] auto get_widget_show_state() -> WidgetShowState&;

[[nodiscard]] auto is_font_reload_scheduled() -> bool;

}  // namespace tactile
