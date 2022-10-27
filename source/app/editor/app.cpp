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
#include "editor/app_context.hpp"
#include "editor/handler/command_event_handler.hpp"
#include "editor/handler/component_event_handler.hpp"
#include "editor/handler/document_event_handler.hpp"
#include "editor/handler/layer_event_handler.hpp"
#include "editor/handler/map_event_handler.hpp"
#include "editor/handler/object_event_handler.hpp"
#include "editor/handler/property_event_handler.hpp"
#include "editor/handler/tileset_event_handler.hpp"
#include "editor/handler/tool_event_handler.hpp"
#include "editor/handler/view_event_handler.hpp"
#include "editor/handler/viewport_event_handler.hpp"
#include "editor/shortcut/shortcuts.hpp"
#include "editor/ui/dialog/dialogs.hpp"
#include "editor/ui/dock/layer/layer_dock.hpp"
#include "editor/ui/dock/tileset/tileset_dock.hpp"
#include "editor/ui/fonts.hpp"
#include "editor/ui/menu/edit_menu.hpp"
#include "editor/ui/style/icons.hpp"
#include "editor/ui/ui.hpp"
#include "editor/ui/viewport/viewport_widget.hpp"
#include "io/load_texture.hpp"
#include "io/proto/history.hpp"
#include "io/proto/preferences.hpp"
#include "io/proto/session.hpp"
#include "model/cmd/commands.hpp"
#include "model/document/map_document.hpp"
#include "model/document/tileset_document.hpp"
#include "model/event/view_events.hpp"
#include "model/model.hpp"

namespace tactile {

App::App()
{
  subscribe_to_events();
  load_default_shortcuts();

  ui::init_dialogs();
  ui::load_icons();
}

App::~App() noexcept
{
  ui::unload_icons();
}

void App::on_startup()
{
  io::load_file_history();

  if (io::get_preferences().restore_last_session) {
    auto& model = get_model();
    io::restore_last_session(model);
  }

  get_window().show();
}

void App::on_shutdown()
{
  save_current_files_to_history();
  io::save_preferences();
  io::save_session(get_model());
  io::save_file_history();

  get_window().hide();
}

void App::on_pre_update()
{
  if (is_font_reload_scheduled()) {
    ui::reload_fonts();
    handled_font_reload();
  }
}

void App::on_update()
{
  get_dispatcher().update();
  get_model().update();
  ui::update_widgets(get_model(), get_dispatcher());
}

void App::on_event(const cen::event_handler& handler)
{
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

  get_dispatcher().sink<ShowSettingsEvent>().connect<&ui::show_settings_dialog>();
  get_dispatcher().sink<QuitEvent>().connect<&App::on_quit>(this);
}

void App::save_current_files_to_history()
{
  get_model().each([](const UUID& id) {
    const auto document = get_model().get_document(id);
    if (document->is_map() && document->has_path()) {
      io::add_file_to_history(document->get_path());
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
      const auto& map_document = get_model().require_active_map();

      const auto& map = map_document.get_map();
      const auto& tilesets = map.tileset_bundle();

      if (const auto tileset_id = tilesets.active_tileset_id()) {
        const auto& tileset_ref = tilesets.get_ref(*tileset_id);
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
