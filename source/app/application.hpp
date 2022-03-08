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

#include <centurion.hpp>
#include <entt/entt.hpp>

#include "core/utils/texture_manager.hpp"
#include "editor/events/command_events.hpp"
#include "editor/events/component_events.hpp"
#include "editor/events/layer_events.hpp"
#include "editor/events/map_events.hpp"
#include "editor/events/misc_events.hpp"
#include "editor/events/object_events.hpp"
#include "editor/events/property_events.hpp"
#include "editor/events/tileset_events.hpp"
#include "editor/events/tool_events.hpp"
#include "editor/events/viewport_events.hpp"
#include "editor/gui/icons.hpp"
#include "editor/gui/widget_manager.hpp"
#include "editor/model.hpp"
#include "tactile.hpp"

namespace tactile {

class app_configuration;

/**
 * \brief Tracks visibility of widgets for the "Toggle UI" feature.
 */
struct widget_show_state final
{
  bool prev_show_layer_dock{};
  bool prev_show_tileset_dock{};
  bool prev_show_properties_dock{};
  bool prev_show_log_dock{};
  bool prev_show_toolbar{};
  bool prev_show_component_dock{};
};

/**
 * \brief Represents the heart of the Tactile application.
 *
 * \details This class handles the main poll/update/render application loop, along with
 * the basic initialization and configuration of all aspects of the Tactile application.
 */
class application final
{
  friend void subscribe_to_events(application&);

 public:
  explicit application(app_configuration* configuration);

  auto run() -> int;

 private:
  app_configuration* mConfiguration{}; /* Non-owning */
  entt::dispatcher mDispatcher;
  cen::keyboard mKeyboard;
  document_model mModel;
  texture_manager mTextures;
  icon_manager mIcons;
  widget_manager mWidgets;
  widget_show_state mWidgetShowState;
  bool mQuit{};

  template <typename Event, auto Slot>
  void connect()
  {
    mDispatcher.sink<Event>().template connect<Slot>(this);
  }

  void on_about_to_exit();
  void save_current_files_to_history();

  void poll_events();
  void on_keyboard_event(cen::keyboard_event event);
  void on_mouse_wheel_event(const cen::mouse_wheel_event& event);

  void update_frame();

  void on_undo();
  void on_redo();
  void on_set_command_capacity(const set_command_capacity_event& event);

  void on_save();
  void on_save_as(const save_as_event& event);
  void on_open_save_as_dialog();
  void on_show_settings();

  void on_show_new_map_dialog();
  void on_show_open_map_dialog();
  void on_show_map_properties();
  void on_create_map(const create_map_event& event);
  void on_close_map(const close_map_event& event);
  void on_open_map(const open_map_event& event);
  void on_select_map(const select_map_event& event);

  void on_select_tool(const select_tool_event& event);
  void on_tool_pressed(const ToolPressedEvent& event);
  void on_tool_dragged(const ToolDraggedEvent& event);
  void on_tool_released(const ToolReleasedEvent& event);
  void on_stamp_sequence(stamp_sequence_event event);
  void on_eraser_sequence(eraser_sequence_event event);
  void on_flood(const flood_event& event);
  void on_add_rectangle(const AddRectangleEvent& event);
  void on_add_ellipse(const AddEllipseEvent& event);
  void on_add_point(const AddPointEvent& event);

  void on_center_viewport();
  void on_offset_viewport(const OffsetViewportEvent& event);
  void on_offset_bound_viewport(const OffsetBoundViewportEvent& event);
  void on_pan_left();
  void on_pan_right();
  void on_pan_up();
  void on_pan_down();
  void on_increase_zoom();
  void on_decrease_zoom();
  void on_reset_zoom();

  void on_show_add_tileset_dialog();
  void on_add_tileset(const add_tileset_event& event);
  void on_remove_tileset(const remove_tileset_event& event);
  void on_select_tileset(const select_tileset_event& event);
  void on_set_tileset_selection(const set_tileset_selection_event& event);
  void on_set_tileset_name(const set_tileset_name_event& event);

  void on_add_row();
  void on_add_column();
  void on_remove_row();
  void on_remove_column();
  void on_resize_map(const resize_map_event& event);
  void on_open_resize_map_dialog();

  void on_add_layer(const add_layer_event& event);
  void on_remove_layer(const remove_layer_event& event);
  void on_select_layer(const select_layer_event& event);
  void on_move_layer_up(const move_layer_up_event& event);
  void on_move_layer_down(const move_layer_down_event& event);
  void on_duplicate_layer(const duplicate_layer_event& event);
  void on_set_layer_opacity(const set_layer_opacity_event& event);
  void on_set_layer_visible(const set_layer_visible_event& event);
  void on_open_rename_layer_dialog(const open_rename_layer_dialog_event& event);
  void on_rename_layer(const rename_layer_event& event);

  void on_set_object_name(const set_object_name_event& event);
  void on_move_object(const move_object_event& event);
  void on_set_object_visibility(const set_object_visibility_event& event);
  void on_set_object_tag(const set_object_tag_event& event);
  void on_spawn_object_context_menu(const spawn_object_context_menu_event& event);

  void on_show_add_property_dialog();
  void on_show_rename_property_dialog(const show_rename_property_dialog_event& event);
  void on_show_change_property_type_dialog(
      const show_change_property_type_dialog_event& event);
  void on_add_property(const add_property_event& event);
  void on_remove_property(const remove_property_event& event);
  void on_rename_property(const rename_property_event& event);
  void on_update_property(const update_property_event& event);
  void on_change_property_type(const change_property_type_event& event);
  void on_inspect_context(const inspect_context_event& event);

  void on_open_component_editor();
  void on_create_component_def(const create_component_def_event& event);
  void on_remove_component_def(const remove_component_def_event& event);
  void on_rename_component_def(const rename_component_def_event& event);
  void on_create_component_attr(const create_component_attr_event& event);
  void on_remove_component_attr(const remove_component_attr_event& event);
  void on_rename_component_attr(const rename_component_attr_event& event);
  void on_duplicate_component_attr(const duplicate_component_attr_event& event);
  void on_set_component_attr_type(const set_component_attr_type_event& event);
  void on_update_component_def_attr(const update_component_def_attr_event& event);

  void on_add_component(const add_component_event& event);
  void on_remove_component(const remove_component_event& event);
  void on_update_component(const update_component_event& event);
  void on_reset_component_values(const reset_component_values_event& event);

  void on_toggle_ui();
  void on_quit();
};

}  // namespace tactile
