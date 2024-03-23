// Copyright (C) 2024 Albin Johansson (GNU General Public License v3.0)

#pragma once

#include <entt/fwd.hpp>

#include "tactile/base/prelude.hpp"

namespace tactile {

class IWindow;

TACTILE_FWD_DECLARE_CLASS(DocumentModel)
TACTILE_FWD_DECLARE_STRUCT(WidgetShowState)

void init_app_context(IWindow* window);

void request_font_reload();
void handled_font_reload();

[[nodiscard]] auto get_window() -> IWindow*;
[[nodiscard]] auto get_dispatcher() -> entt::dispatcher&;
[[nodiscard]] auto get_model() -> DocumentModel&;
[[nodiscard]] auto get_widget_show_state() -> WidgetShowState&;

[[nodiscard]] auto is_font_reload_scheduled() -> bool;

}  // namespace tactile
