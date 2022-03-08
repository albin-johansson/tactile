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

#include "application_events.hpp"

#include "application.hpp"
#include "editor/events/map_events.hpp"
#include "editor/events/misc_events.hpp"
#include "editor/events/tileset_events.hpp"
#include "editor/events/viewport_events.hpp"

namespace tactile {

using appl = application;

void subscribe_to_events(appl& app)
{
  app.connect<undo_event, &appl::on_undo>();
  app.connect<redo_event, &appl::on_redo>();
  app.connect<set_command_capacity_event, &appl::on_set_command_capacity>();

  app.connect<save_event, &appl::on_save>();
  app.connect<save_as_event, &appl::on_save_as>();
  app.connect<open_save_as_dialog_event, &appl::on_open_save_as_dialog>();
  app.connect<show_settings_event, &appl::on_show_settings>();

  app.connect<show_new_map_dialog_event, &appl::on_show_new_map_dialog>();
  app.connect<show_open_map_dialog_event, &appl::on_show_open_map_dialog>();
  app.connect<inspect_map_event, &appl::on_show_map_properties>();
  app.connect<create_map_event, &appl::on_create_map>();
  app.connect<close_map_event, &appl::on_close_map>();
  app.connect<open_map_event, &appl::on_open_map>();
  app.connect<select_map_event, &appl::on_select_map>();

  app.connect<select_tool_event, &appl::on_select_tool>();
  app.connect<ToolPressedEvent, &appl::on_tool_pressed>();
  app.connect<ToolDraggedEvent, &appl::on_tool_dragged>();
  app.connect<ToolReleasedEvent, &appl::on_tool_released>();
  app.connect<stamp_sequence_event, &appl::on_stamp_sequence>();
  app.connect<eraser_sequence_event, &appl::on_eraser_sequence>();
  app.connect<flood_event, &appl::on_flood>();
  app.connect<AddRectangleEvent, &appl::on_add_rectangle>();
  app.connect<AddPointEvent, &appl::on_add_point>();

  app.connect<CenterViewportEvent, &appl::on_center_viewport>();
  app.connect<OffsetViewportEvent, &appl::on_offset_viewport>();
  app.connect<OffsetBoundViewportEvent, &appl::on_offset_bound_viewport>();
  app.connect<PanLeftEvent, &appl::on_pan_left>();
  app.connect<PanRightEvent, &appl::on_pan_right>();
  app.connect<PanUpEvent, &appl::on_pan_up>();
  app.connect<PanDownEvent, &appl::on_pan_down>();
  app.connect<IncreaseZoomEvent, &appl::on_increase_zoom>();
  app.connect<DecreaseZoomEvent, &appl::on_decrease_zoom>();
  app.connect<ResetZoomEvent, &appl::on_reset_zoom>();

  app.connect<show_add_tileset_dialog_event, &appl::on_show_add_tileset_dialog>();
  app.connect<add_tileset_event, &appl::on_add_tileset>();
  app.connect<remove_tileset_event, &appl::on_remove_tileset>();
  app.connect<select_tileset_event, &appl::on_select_tileset>();
  app.connect<set_tileset_selection_event, &appl::on_set_tileset_selection>();
  app.connect<set_tileset_name_event, &appl::on_set_tileset_name>();

  app.connect<add_row_event, &appl::on_add_row>();
  app.connect<add_column_event, &appl::on_add_column>();
  app.connect<remove_row_event, &appl::on_remove_row>();
  app.connect<remove_column_event, &appl::on_remove_column>();
  app.connect<resize_map_event, &appl::on_resize_map>();
  app.connect<open_resize_map_dialog_event, &appl::on_open_resize_map_dialog>();

  app.connect<add_layer_event, &appl::on_add_layer>();
  app.connect<remove_layer_event, &appl::on_remove_layer>();
  app.connect<select_layer_event, &appl::on_select_layer>();
  app.connect<move_layer_up_event, &appl::on_move_layer_up>();
  app.connect<move_layer_down_event, &appl::on_move_layer_down>();
  app.connect<duplicate_layer_event, &appl::on_duplicate_layer>();
  app.connect<set_layer_opacity_event, &appl::on_set_layer_opacity>();
  app.connect<set_layer_visible_event, &appl::on_set_layer_visible>();
  app.connect<open_rename_layer_dialog_event, &appl::on_open_rename_layer_dialog>();
  app.connect<rename_layer_event, &appl::on_rename_layer>();

  app.connect<set_object_name_event, &appl::on_set_object_name>();
  app.connect<move_object_event, &appl::on_move_object>();
  app.connect<set_object_visibility_event, &appl::on_set_object_visibility>();
  app.connect<set_object_tag_event, &appl::on_set_object_tag>();
  app.connect<spawn_object_context_menu_event, &appl::on_spawn_object_context_menu>();

  app.connect<show_add_property_dialog_event, &appl::on_show_add_property_dialog>();
  app.connect<show_rename_property_dialog_event, &appl::on_show_rename_property_dialog>();
  app.connect<show_change_property_type_dialog_event,
              &appl::on_show_change_property_type_dialog>();
  app.connect<add_property_event, &appl::on_add_property>();
  app.connect<remove_property_event, &appl::on_remove_property>();
  app.connect<rename_property_event, &appl::on_rename_property>();
  app.connect<update_property_event, &appl::on_update_property>();
  app.connect<change_property_type_event, &appl::on_change_property_type>();
  app.connect<inspect_context_event, &appl::on_inspect_context>();

  app.connect<open_component_editor_event, &appl::on_open_component_editor>();
  app.connect<create_component_def_event, &appl::on_create_component_def>();
  app.connect<remove_component_def_event, &appl::on_remove_component_def>();
  app.connect<rename_component_def_event, &appl::on_rename_component_def>();
  app.connect<create_component_attr_event, &appl::on_create_component_attr>();
  app.connect<remove_component_attr_event, &appl::on_remove_component_attr>();
  app.connect<rename_component_attr_event, &appl::on_rename_component_attr>();
  app.connect<duplicate_component_attr_event, &appl::on_duplicate_component_attr>();
  app.connect<set_component_attr_type_event, &appl::on_set_component_attr_type>();
  app.connect<update_component_def_attr_event, &appl::on_update_component_def_attr>();

  app.connect<add_component_event, &appl::on_add_component>();
  app.connect<remove_component_event, &appl::on_remove_component>();
  app.connect<update_component_event, &appl::on_update_component>();
  app.connect<reset_component_values_event, &appl::on_reset_component_values>();

  app.connect<toggle_ui_event, &appl::on_toggle_ui>();
  app.connect<quit_event, &appl::on_quit>();
}

}  // namespace tactile
