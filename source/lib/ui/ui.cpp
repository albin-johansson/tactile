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

#include "ui.hpp"

#include <imgui_internal.h>

#include "io/file_dialog.hpp"
#include "model/documents/document_system.hpp"
#include "model/events/map_events.hpp"
#include "model/events/menu_events.hpp"
#include "model/model_view.hpp"
#include "ui/dialog/about_dialog.hpp"
#include "ui/dialog/credits_dialog.hpp"
#include "ui/dialog/godot_export_dialog.hpp"
#include "ui/dialog/map_parse_error_dialog.hpp"
#include "ui/dialog/new_map_dialog.hpp"
#include "ui/dialog/resize_map_dialog.hpp"
#include "ui/dialog/settings_dialog.hpp"
#include "ui/dock/animation/animation_dock.hpp"
#include "ui/dock/comp/component_dock.hpp"
#include "ui/dock/comp/component_editor_dialog.hpp"
#include "ui/dock/dock_space.hpp"
#include "ui/dock/editor/editor_dock.hpp"
#include "ui/dock/layer/layer_dock.hpp"
#include "ui/dock/log/log_dock.hpp"
#include "ui/dock/property/property_dock.hpp"
#include "ui/dock/tileset/tileset_dock.hpp"
#include "ui/menu/menu_bar.hpp"
#include "ui/style/alignment.hpp"
#include "ui/style/colors.hpp"
#include "ui/widget_state.hpp"

namespace tactile {
namespace {

void _poll_shortcut(ModelView model, const MenuAction action, const ImGuiKeyChord chord)
{
  if (ImGui::Shortcut(chord, ImGuiKeyOwner_Any, ImGuiInputFlags_RouteGlobalLow) &&
      model.is_available(action)) {
    model.enqueue<MenuActionEvent>(action);
  }
}

}  // namespace

void poll_global_shortcuts(ModelView model)
{
  // clang-format off
  _poll_shortcut(model, MenuAction::NewMap, ImGuiMod_Shortcut | ImGuiKey_N);
  _poll_shortcut(model, MenuAction::OpenMap, ImGuiMod_Shortcut | ImGuiKey_O);
  _poll_shortcut(model, MenuAction::Save, ImGuiMod_Shortcut | ImGuiKey_S);
  _poll_shortcut(model, MenuAction::SaveAs, ImGuiMod_Shortcut | ImGuiMod_Shift | ImGuiKey_S);

  _poll_shortcut(model, MenuAction::Undo, ImGuiMod_Shortcut | ImGuiKey_Z);
  _poll_shortcut(model, MenuAction::Redo, ImGuiMod_Shortcut | ImGuiKey_Y);
  _poll_shortcut(model, MenuAction::OpenComponentEditor, ImGuiMod_Shortcut | ImGuiMod_Shift | ImGuiKey_C);
  _poll_shortcut(model, MenuAction::OpenSettings, ImGuiMod_Shortcut | ImGuiKey_Comma);

  _poll_shortcut(model, MenuAction::CenterViewport, ImGuiMod_Shift | ImGuiKey_Space);
  _poll_shortcut(model, MenuAction::IncreaseZoom, ImGuiMod_Shortcut | ImGuiKey_0);
  _poll_shortcut(model, MenuAction::DecreaseZoom, ImGuiMod_Shortcut | ImGuiKey_9);
  _poll_shortcut(model, MenuAction::IncreaseFontSize, ImGuiMod_Shortcut | ImGuiMod_Shift | ImGuiKey_0);
  _poll_shortcut(model, MenuAction::DecreaseFontSize, ImGuiMod_Shortcut | ImGuiMod_Shift | ImGuiKey_9);
  _poll_shortcut(model, MenuAction::PanUp, ImGuiMod_Shortcut | ImGuiMod_Shift | ImGuiKey_UpArrow);
  _poll_shortcut(model, MenuAction::PanDown, ImGuiMod_Shortcut | ImGuiMod_Shift | ImGuiKey_DownArrow);
  _poll_shortcut(model, MenuAction::PanLeft, ImGuiMod_Shortcut | ImGuiMod_Shift | ImGuiKey_LeftArrow);
  _poll_shortcut(model, MenuAction::PanRight, ImGuiMod_Shortcut | ImGuiMod_Shift | ImGuiKey_RightArrow);

  _poll_shortcut(model, MenuAction::HighlightLayer, ImGuiKey_H);
  _poll_shortcut(model, MenuAction::ToggleUi, ImGuiKey_Tab);

  _poll_shortcut(model, MenuAction::CreateTileset, ImGuiMod_Shortcut | ImGuiKey_T);
  _poll_shortcut(model, MenuAction::AddRow, ImGuiMod_Alt | ImGuiKey_R);
  _poll_shortcut(model, MenuAction::AddColumn, ImGuiMod_Alt | ImGuiKey_C);
  _poll_shortcut(model, MenuAction::RemoveRow, ImGuiMod_Alt | ImGuiMod_Shift | ImGuiKey_R);
  _poll_shortcut(model, MenuAction::RemoveColumn, ImGuiMod_Alt | ImGuiMod_Shift | ImGuiKey_C);
  // clang-format on
}

void render_ui(ModelView model, WidgetState& widgets)
{
  const auto& registry = model.get_registry();
  auto& dispatcher = model.get_dispatcher();

  update_dynamic_color_cache();  // TODO service?
  update_dock_space(registry, widgets.dock_space);

  push_menu_bar(model, widgets.menu_bar);
  push_editor_dock_widget(model, widgets.editor_dock);

  if (is_layer_dock_enabled(registry)) {
    push_layer_dock_widget(model, widgets.layer_dock);
  }

  if (is_tileset_dock_enabled(registry)) {
    push_tileset_dock_widget(model, widgets.tileset_dock);
  }

  if (is_animation_dock_enabled(registry)) {
    push_animation_dock_widget(model, widgets.animation_dock);
  }

  if (sys::has_active_document(registry)) {
    push_property_dock_widget(model, widgets.property_dock);
    push_component_dock_widget(model);
  }

  push_log_dock_widget(model, widgets.log_dock);

  push_new_map_dialog(model, widgets.new_map_dialog);
  push_new_tileset_dialog(model, widgets.new_tileset_dialog);
  push_resize_map_dialog(model, widgets.resize_map_dialog);
  push_component_editor_dialog(model, widgets.component_editor_dialog);
  push_godot_export_dialog(model, widgets.godot_export_dialog);
  push_settings_dialog(model, widgets.settings_dialog);
  push_map_parse_error_dialog(model, widgets.map_parse_error_dialog);
  push_credits_dialog(model, widgets.credits_dialog);
  push_about_dialog(model, widgets.about_dialog);

  if (widgets.should_open_map_file_dialog) {
    auto dialog = FileDialog::open_map();
    if (dialog.is_okay()) {
      dispatcher.enqueue<OpenMapEvent>(dialog.path());
    }
    widgets.should_open_map_file_dialog = false;
  }

  if (widgets.should_open_about_imgui_dialog) {
    ui::center_next_window_on_appearance();
    ImGui::ShowAboutWindow(&widgets.should_open_about_imgui_dialog);
  }
}

}  // namespace tactile
