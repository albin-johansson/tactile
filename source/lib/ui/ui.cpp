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

#include "ui/dialog/about_dialog.hpp"
#include "ui/dialog/credits_dialog.hpp"
#include "ui/dialog/godot_export_dialog.hpp"
#include "ui/dialog/map_parse_error_dialog.hpp"
#include "ui/dialog/new_map_dialog.hpp"
#include "ui/dialog/resize_map_dialog.hpp"
#include "ui/dialog/settings_dialog.hpp"
#include "ui/dock/animation/animation_dock.hpp"
#include "ui/dock/comp/component_dock.hpp"
#include "ui/dock/comp/component_editor.hpp"
#include "ui/dock/dock_space.hpp"
#include "ui/dock/editor/editor_dock.hpp"
#include "ui/dock/layer/layer_dock.hpp"
#include "ui/dock/log/log_dock.hpp"
#include "ui/dock/property/property_dock.hpp"
#include "ui/dock/tileset/tileset_dock.hpp"
#include "ui/menu/menu_bar.hpp"
#include "ui/style/colors.hpp"

namespace tactile::ui {

void render_ui(const Model& model, WidgetState& widgets, Dispatcher& dispatcher)
{
  update_dynamic_color_cache();
  update_dock_space(model);

  push_menu_bar(model, dispatcher);

  if (is_layer_dock_enabled(model)) {
    push_layer_dock_widget(model, widgets.layer_dock, dispatcher);
  }

  if (is_tileset_dock_enabled(model)) {
    push_tileset_dock_widget(model, widgets.tileset_dock, dispatcher);
  }

  if (is_animation_dock_enabled(model)) {
    push_animation_dock_widget(model, widgets.animation_dock, dispatcher);
  }

  push_property_dock_widget(model, widgets.property_dock, dispatcher);
  push_component_dock_widget(model, dispatcher);
  push_log_dock_widget(model, widgets.log_dock, dispatcher);

  push_editor_dock_widget(model,
                          widgets.editor_dock,
                          widgets.tileset_viewport,
                          dispatcher);

  push_new_map_dialog(model, widgets.new_map_dialog, dispatcher);
  push_new_tileset_dialog(model, widgets.new_tileset_dialog, dispatcher);
  push_resize_map_dialog(model, widgets.resize_map_dialog, dispatcher);
  push_component_editor_dialog(model, widgets.component_editor_dialog, dispatcher);
  push_godot_export_dialog(model, widgets.godot_export_dialog, dispatcher);

  show_settings_dialog(model, kNullEntity, dispatcher);
  show_map_parse_error_dialog(model, kNullEntity, dispatcher);
  show_credits_dialog(model, kNullEntity, dispatcher);
  show_about_dialog(model, kNullEntity, dispatcher);

  // TODO
  //  if (gOpenMapFileDialog) {
  //    auto dialog = FileDialog::open_map();
  //    if (dialog.is_okay()) {
  //      dispatcher.enqueue<OpenMapEvent>(dialog.path());
  //    }
  //  }

  // TODO
  //  if (gOpenAboutImGuiDialog) {
  //    center_next_window_on_appearance();
  //    ImGui::ShowAboutWindow(&gOpenAboutImGuiDialog);
  //  }
}

}  // namespace tactile::ui
