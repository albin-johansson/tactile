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

#include "app.hpp"

#include <entt/signal/dispatcher.hpp>
#include <imgui.h>
#include <imgui_internal.h>

#include "core/layer/object_layer.hpp"
#include "core/tile/tileset_bundle.hpp"
#include "handlers/event_handlers.hpp"
#include "io/load_texture.hpp"
#include "io/proto/history.hpp"
#include "io/proto/session.hpp"
#include "io/proto/settings.hpp"
#include "model/cmd/commands.hpp"
#include "model/document/map_document.hpp"
#include "model/document/tileset_document.hpp"
#include "model/event/menu_events.hpp"
#include "model/event/view_events.hpp"
#include "model/model.hpp"
#include "model/settings.hpp"
#include "runtime/app_context.hpp"
#include "ui/dialog/settings_dialog.hpp"
#include "ui/dock/layer/layer_dock.hpp"
#include "ui/dock/tileset/tileset_dock.hpp"
#include "ui/imgui_context.hpp"
#include "ui/menu/menu.hpp"
#include "ui/shortcut/shortcuts.hpp"
#include "ui/style/icons.hpp"
#include "ui/ui.hpp"
#include "ui/viewport/viewport_widget.hpp"

namespace tactile {

App::App(cen::window& window)
{
  init_app_context(window);

  subscribe_to_events();
  load_default_shortcuts();
  ui::load_icons();

  window.maximize();
}

App::~App() noexcept
{
  ui::unload_icons();
}

void App::on_startup()
{
  load_file_history_from_disk();

  if (get_settings().test_flag(SETTINGS_RESTORE_LAST_SESSION_BIT)) {
    auto& model = get_model();
    load_session_from_disk(model);
  }

  get_window().show();
}

void App::on_shutdown()
{
  add_open_documents_to_file_history();

  save_settings_to_disk(get_settings());
  save_session_to_disk(get_model());
  save_file_history_to_disk();

  get_window().hide();
}

void App::on_pre_update()
{
  if (is_font_reload_scheduled()) {
    ImGuiContext::reload_fonts();
    handled_font_reload();
  }
}

void App::on_update()
{
  auto& model = get_model();
  auto& dispatcher = get_dispatcher();

  dispatcher.update();
  model.update();
  update_menus(model);
  ui::update_widgets(model, dispatcher);
}

void App::on_event(const cen::event_handler& handler)
{
  if (const auto type = handler.raw_type(); type > SDL_USEREVENT) {
    get_dispatcher().trigger(MenuItemEvent {static_cast<MenuAction>(*type)});
    return;
  }

  switch (handler.type().value()) {
    case cen::event_type::key_up:
      [[fallthrough]];
    case cen::event_type::key_down:
      on_keyboard_event(handler.get<cen::keyboard_event>());
      break;

    case cen::event_type::mouse_wheel:
      on_mouse_wheel_event(handler.get<cen::mouse_wheel_event>());
      break;

    default:
      break;
  }
}

void App::subscribe_to_events()
{
  install_menu_event_handler();
  install_command_event_handler();
  install_document_event_handler();
  install_map_event_handler();
  install_tileset_event_handler();
  install_layer_event_handler();
  install_object_event_handler();
  install_tool_event_handler();
  install_viewport_event_handler();
  install_property_event_handler();
  install_component_event_handler();
  install_view_event_handler();

  get_dispatcher().sink<ShowSettingsEvent>().connect<&ui::open_settings_dialog>();
  get_dispatcher().sink<QuitEvent>().connect<&App::on_quit>(this);
}

void App::add_open_documents_to_file_history()
{
  get_model().each([](const UUID& document_id) {
    const auto& document = get_model().get_document(document_id);
    if (document.is_map() && document.has_path()) {
      add_to_file_history(document.get_path());
    }
  });
}

void App::on_keyboard_event(cen::keyboard_event event)
{
  // We don't care about these modifiers, they are just noise.
  event.set_modifier(cen::key_mod::caps, false);
  event.set_modifier(cen::key_mod::num, false);
  event.set_modifier(cen::key_mod::mode, false);

  update_shortcuts(get_model(), event, get_dispatcher());
}

void App::on_mouse_wheel_event(const cen::mouse_wheel_event& event)
{
  // There doesn't seem to be a good way to handle mouse "wheel" events using the public
  // ImGui APIs. Otherwise, it would be nicer to keep this code closer to the actual
  // widgets.

  const auto* document = get_model().active_document();
  if (document && !ImGui::GetTopMostPopupModal()) {
    if (ui::is_mouse_within_viewport()) {
      ui::viewport_widget_mouse_wheel_event_handler(document->get_viewport(),
                                                    get_dispatcher(),
                                                    event);
    }
    else if (document->is_map() && ui::is_tileset_dock_hovered()) {
      const auto& map_document = get_model().require_active_map_document();

      const auto& map = map_document.get_map();
      const auto& tilesets = map.get_tileset_bundle();

      if (const auto tileset_id = tilesets.get_active_tileset_id()) {
        const auto& tileset_ref = tilesets.get_tileset_ref(*tileset_id);
        ui::tileset_dock_mouse_wheel_event_handler(tileset_ref, event, get_dispatcher());
      }
    }
  }
}

void App::on_quit()
{
  stop();
}

}  // namespace tactile
