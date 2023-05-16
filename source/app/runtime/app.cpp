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

#include "app.hpp"

#include <utility>  // move

#include <core/texture.hpp>
#include <imgui.h>
#include <imgui_internal.h>
#include <spdlog/spdlog.h>

#include "cmd/commands.hpp"
#include "core/map.hpp"
#include "io/directories.hpp"
#include "io/proto/history.hpp"
#include "io/proto/session.hpp"
#include "io/proto/settings.hpp"
#include "model/context.hpp"
#include "model/document.hpp"
#include "model/event/menu_events.hpp"
#include "model/event/view_events.hpp"
#include "model/file_history.hpp"
#include "model/model.hpp"
#include "model/systems/texture_system.hpp"
#include "model/systems/widget_system.hpp"
#include "runtime/app_context.hpp"
#include "ui/dialog/about_dialog.hpp"
#include "ui/dialog/create_map_dialog.hpp"
#include "ui/dialog/credits_dialog.hpp"
#include "ui/dialog/godot_export_dialog.hpp"
#include "ui/dialog/map_parse_error_dialog.hpp"
#include "ui/dialog/resize_map_dialog.hpp"
#include "ui/dialog/settings_dialog.hpp"
#include "ui/dock/animation/animation_dock.hpp"
#include "ui/dock/comp/component_dock.hpp"
#include "ui/dock/comp/component_editor.hpp"
#include "ui/dock/dock_space.hpp"
#include "ui/dock/layer/layer_dock.hpp"
#include "ui/dock/log/log_dock.hpp"
#include "ui/dock/property/property_dock.hpp"
#include "ui/dock/tileset/dialogs/create_tileset_dialog.hpp"
#include "ui/dock/tileset/tileset_dock.hpp"
#include "ui/menu/menu.hpp"
#include "ui/menu/menu_bar.hpp"
#include "ui/shortcut/shortcuts.hpp"
#include "ui/style/colors.hpp"
#include "ui/viewport/viewport_widget.hpp"

namespace tactile {
namespace {

void _check_for_missing_layout_file()
{
  const auto& ini = get_widget_ini_path();
  if (!fs::exists(ini)) {
    spdlog::warn("Resetting layout because 'imgui.ini' is missing...");
    ui::reset_layout();

    const auto str = ini.string();
    ImGui::SaveIniSettingsToDisk(str.c_str());
  }
}

}  // namespace

App::App()
{
  subscribe_to_events();
  init_default_shortcuts();
}

void App::on_startup()
{
  auto& model = get_global_model();

  // Configure settings and load the language files
  auto& settings = get_global_settings();
  settings.copy_values_from(load_settings_from_disk());
  settings.print();

  load_languages();
  init_menus();

  if (auto history = load_file_history_from_disk()) {
    set_file_history(std::move(*history));
  }

  if (settings.test_flag(SETTINGS_RESTORE_LAST_SESSION_BIT)) {
    load_session_from_disk(model);
  }

  auto& style = ImGui::GetStyle();
  style.WindowBorderSize = settings.test_flag(SETTINGS_WINDOW_BORDER_BIT) ? 1.0f : 0.0f;
  apply_theme(style, settings.get_theme(), settings.get_theme_saturation());

  auto& icons = model.get<Icons>();
  icons.tactile_icon = sys::create_texture(model, find_resource("assets/icon.png"));

  uint32 idx = 0;
  sys::add_widget(model, idx++, ui::show_menu_bar);

  // Dock widgets
  sys::add_widget(model, idx++, ui::show_layer_dock, ui::is_layer_dock_enabled);
  sys::add_widget(model, idx++, ui::show_tileset_dock, ui::is_tileset_dock_enabled);
  sys::add_widget(model, idx++, ui::show_animation_dock, ui::is_animation_dock_enabled);
  sys::add_widget(model, idx++, ui::show_property_dock);
  sys::add_widget(model, idx++, ui::show_component_dock);
  sys::add_widget(model, idx++, ui::show_log_dock);
  sys::add_widget(model, idx++, ui::show_viewport_dock);

  // Dialogs
  sys::add_widget(model, idx++, ui::show_component_editor_dialog);
  sys::add_widget(model, idx++, ui::show_create_map_dialog);
  sys::add_widget(model, idx++, ui::show_create_tileset_dialog);
  sys::add_widget(model, idx++, ui::show_resize_map_dialog);
  sys::add_widget(model, idx++, ui::show_godot_export_dialog);
  sys::add_widget(model, idx++, ui::show_settings_dialog);
  sys::add_widget(model, idx++, ui::show_map_parse_error_dialog);
  sys::add_widget(model, idx++, ui::show_credits_dialog);
  sys::add_widget(model, idx, ui::show_about_dialog);

  sys::sort_widgets(model);
}

void App::on_shutdown()
{
  add_open_documents_to_file_history();
  save_settings_to_disk(get_global_settings());
  save_session_to_disk(get_global_model());
  save_file_history_to_disk(get_file_history());
}

void App::on_pre_update()
{
  if (is_font_reload_scheduled()) {
    handled_font_reload();
  }
}

void App::on_update()
{
  auto& model = get_global_model();
  auto& dispatcher = get_global_dispatcher();

  dispatcher.update();

  ui::update_dynamic_color_cache();
  ui::update_dock_space();

  sys::render_widgets(model, dispatcher);

  _check_for_missing_layout_file();
}

void App::on_event(const cen::event_handler& handler)
{
  if (const auto type = handler.raw_type(); type > SDL_USEREVENT) {
    get_global_dispatcher().trigger(MenuItemEvent {static_cast<MenuAction>(*type)});
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
  //  install_menu_event_handler();
  //  install_command_event_handler();
  //  install_document_event_handler();
  //  install_map_event_handler();
  //  install_tileset_event_handler();
  //  install_layer_event_handler();
  //  install_object_event_handler();
  //  install_tool_event_handler();
  //  install_viewport_event_handler();
  //  install_property_event_handler();
  //  install_component_event_handler();
  //  install_view_event_handler();

  auto& dispatcher = get_global_dispatcher();
  dispatcher.sink<ShowSettingsEvent>().connect<&ui::open_settings_dialog>();
  dispatcher.sink<QuitEvent>().connect<&App::on_quit>(this);
}

void App::add_open_documents_to_file_history()
{
  for (auto [document_entity, document]: get_global_model().each<Document>()) {
    if (document.type == DocumentType::Map && document.path.has_value()) {
      add_to_file_history(*document.path);
    }
  }
}

void App::on_keyboard_event(cen::keyboard_event event)
{
  // We don't care about these modifiers, they are just noise.
  event.set_modifier(cen::key_mod::caps, false);
  event.set_modifier(cen::key_mod::num, false);
  event.set_modifier(cen::key_mod::mode, false);

  update_shortcuts(event, get_global_dispatcher());
}

void App::on_mouse_wheel_event(const cen::mouse_wheel_event& event)
{
  // There doesn't seem to be a good way to handle mouse "wheel" events using the public
  // ImGui APIs. Otherwise, it would be nicer to keep this code closer to the actual
  // widgets.

  const auto& model = get_global_model();

  const auto& document_context = model.get<CDocumentContext>();
  const auto document_entity = document_context.active_document;

  if (document_entity != kNullEntity && !ImGui::GetTopMostPopupModal()) {
    const auto& document = model.get<Document>(document_entity);
    const auto& document_viewport = model.get<Viewport>(document_entity);

    if (ui::is_mouse_within_viewport()) {
      ui::viewport_widget_mouse_wheel_event_handler(document_viewport,
                                                    get_global_dispatcher(),
                                                    event);
    }
    else if (document.type == DocumentType::Map && ui::is_tileset_dock_hovered()) {
      const auto& map_document = model.get<MapDocument>(document_entity);
      const auto& map = model.get<Map>(map_document.map);

      if (map_document.active_tileset != kNullEntity) {
        const auto& attached_tileset =
            model.get<AttachedTileset>(map_document.active_tileset);

        // TODO ui::tileset_dock_mouse_wheel_event_handler(tileset_ref, event,
        // get_dispatcher());
      }
    }
  }
}

void App::on_quit()
{
  mShouldStop = true;
}

}  // namespace tactile
