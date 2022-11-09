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

#include "cfg/configuration.hpp"
#include "editor/widget_show_state.hpp"
#include "misc/panic.hpp"
#include "model/model.hpp"

namespace tactile {
namespace {

inline AppCfg* app_cfg {};
inline entt::dispatcher app_dispatcher;
inline DocumentModel app_model;
inline WidgetShowState app_widget_show_state;
inline constinit bool app_font_reload_scheduled = false;

}  // namespace

void init_app_context(AppCfg* cfg)
{
  if (!cfg) {
    throw TactileError {"Invalid application configuration!"};
  }

  app_cfg = cfg;
}

void request_font_reload()
{
  app_font_reload_scheduled = true;
}

void handled_font_reload()
{
  app_font_reload_scheduled = false;
}

auto get_window() -> cen::window&
{
  return app_cfg->window();
}

auto get_dispatcher() -> entt::dispatcher&
{
  return app_dispatcher;
}

auto get_model() -> DocumentModel&
{
  return app_model;
}

auto get_widget_show_state() -> WidgetShowState&
{
  return app_widget_show_state;
}

auto is_font_reload_scheduled() -> bool
{
  return app_font_reload_scheduled;
}

}  // namespace tactile
