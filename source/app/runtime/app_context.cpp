// Copyright (C) 2024 Albin Johansson (GNU General Public License v3.0)

#include "app_context.hpp"

#include <entt/signal/dispatcher.hpp>

#include "model/model.hpp"
#include "tactile/core/debug/assert.hpp"
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

inline AppState gAppState;

}  // namespace

void init_app_context(cen::window& window)
{
  gAppState.window = &window;
}

void request_font_reload()
{
  gAppState.font_reload_scheduled = true;
}

void handled_font_reload()
{
  gAppState.font_reload_scheduled = false;
}

auto get_window() -> cen::window&
{
  TACTILE_ASSERT(gAppState.window != nullptr);
  return *gAppState.window;
}

auto get_dispatcher() -> entt::dispatcher&
{
  return gAppState.dispatcher;
}

auto get_model() -> DocumentModel&
{
  return gAppState.model;
}

auto get_widget_show_state() -> WidgetShowState&
{
  return gAppState.widget_show_state;
}

auto is_font_reload_scheduled() -> bool
{
  return gAppState.font_reload_scheduled;
}

}  // namespace tactile
