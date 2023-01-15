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

#include "app_context.hpp"

#include <entt/signal/dispatcher.hpp>

#include "common/debug/assert.hpp"
#include "model/model.hpp"
#include "ui/widget_show_state.hpp"

namespace tactile {
namespace {

struct AppState final {
  cen::window* window {};
  entt::dispatcher dispatcher;
  DocumentModel model;
  WidgetShowState widget_show_state;
  bool font_reload_scheduled : 1 {};
};

inline AppState app_state;

}  // namespace

void init_app_context(cen::window& window)
{
  app_state.window = &window;
}

void request_font_reload()
{
  app_state.font_reload_scheduled = true;
}

void handled_font_reload()
{
  app_state.font_reload_scheduled = false;
}

auto get_window() -> cen::window&
{
  TACTILE_ASSERT(app_state.window != nullptr);
  return *app_state.window;
}

auto get_dispatcher() -> entt::dispatcher&
{
  return app_state.dispatcher;
}

auto get_model() -> DocumentModel&
{
  return app_state.model;
}

auto get_widget_show_state() -> WidgetShowState&
{
  return app_state.widget_show_state;
}

auto is_font_reload_scheduled() -> bool
{
  return app_state.font_reload_scheduled;
}

}  // namespace tactile
