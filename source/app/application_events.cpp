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

using App = Application;

void subscribe_to_events(App& app)
{
  app.connect<UndoEvent, &App::on_undo>();
  app.connect<RedoEvent, &App::on_redo>();
  app.connect<SetCommandCapacityEvent, &App::on_set_command_capacity>();

  app.connect<SaveEvent, &App::on_save>();
  app.connect<SaveAsEvent, &App::on_save_as>();
  app.connect<OpenSaveAsDialogEvent, &App::on_open_save_as_dialog>();
  app.connect<ShowSettingsEvent, &App::on_show_settings>();

  app.connect<ShowNewMapDialogEvent, &App::on_show_new_map_dialog>();
  app.connect<ShowOpenMapDialogEvent, &App::on_show_open_map_dialog>();
  app.connect<InspectMapEvent, &App::on_show_map_properties>();
  app.connect<CreateMapEvent, &App::on_create_map>();
  app.connect<CloseMapEvent, &App::on_close_map>();
  app.connect<OpenMapEvent, &App::on_open_map>();
  app.connect<SelectMapEvent, &App::on_select_map>();

  app.connect<SelectToolEvent, &App::on_select_tool>();
  app.connect<ToolPressedEvent, &App::on_tool_pressed>();
  app.connect<ToolDraggedEvent, &App::on_tool_dragged>();
  app.connect<ToolReleasedEvent, &App::on_tool_released>();
  app.connect<ToolEnteredEvent, &App::on_tool_entered>();
  app.connect<ToolExitedEvent, &App::on_tool_exited>();

  app.connect<StampSequenceEvent, &App::on_stamp_sequence>();
  app.connect<EraserSequenceEvent, &App::on_eraser_sequence>();
  app.connect<FloodEvent, &App::on_flood>();
  app.connect<AddRectangleEvent, &App::on_add_rectangle>();
  app.connect<AddEllipseEvent, &App::on_add_ellipse>();
  app.connect<AddPointEvent, &App::on_add_point>();

  app.connect<CenterViewportEvent, &App::on_center_viewport>();
  app.connect<OffsetViewportEvent, &App::on_offset_viewport>();
  app.connect<OffsetBoundViewportEvent, &App::on_offset_bound_viewport>();
  app.connect<PanLeftEvent, &App::on_pan_left>();
  app.connect<PanRightEvent, &App::on_pan_right>();
  app.connect<PanUpEvent, &App::on_pan_up>();
  app.connect<PanDownEvent, &App::on_pan_down>();
  app.connect<IncreaseZoomEvent, &App::on_increase_zoom>();
  app.connect<DecreaseZoomEvent, &App::on_decrease_zoom>();
  app.connect<ResetZoomEvent, &App::on_reset_zoom>();

  app.connect<ShowTilesetCreationDialogEvent, &App::on_show_tileset_creation_dialog>();
  app.connect<AddTilesetEvent, &App::on_add_tileset>();
  app.connect<RemoveTilesetEvent, &App::on_remove_tileset>();
  app.connect<SelectTilesetEvent, &App::on_select_tileset>();
  app.connect<SetTilesetSelectionEvent, &App::on_set_tileset_selection>();
  app.connect<SetTilesetNameEvent, &App::on_set_tileset_name>();

  app.connect<AddRowEvent, &App::on_add_row>();
  app.connect<AddColumnEvent, &App::on_add_column>();
  app.connect<RemoveRowEvent, &App::on_remove_row>();
  app.connect<RemoveColumnEvent, &App::on_remove_column>();
  app.connect<ResizeMapEvent, &App::on_resize_map>();
  app.connect<OpenResizeMapDialogEvent, &App::on_open_resize_map_dialog>();

  app.connect<AddLayerEvent, &App::on_add_layer>();
  app.connect<RemoveLayerEvent, &App::on_remove_layer>();
  app.connect<SelectLayerEvent, &App::on_select_layer>();
  app.connect<MoveLayerUpEvent, &App::on_move_layer_up>();
  app.connect<MoveLayerDownEvent, &App::on_move_layer_down>();
  app.connect<DuplicateLayerEvent, &App::on_duplicate_layer>();
  app.connect<SetLayerOpacityEvent, &App::on_set_layer_opacity>();
  app.connect<SetLayerVisibleEvent, &App::on_set_layer_visible>();
  app.connect<OpenRenameLayerDialogEvent, &App::on_open_rename_layer_dialog>();
  app.connect<RenameLayerEvent, &App::on_rename_layer>();

  app.connect<SetObjectNameEvent, &App::on_set_object_name>();
  app.connect<MoveObjectEvent, &App::on_move_object>();
  app.connect<SetObjectVisibilityEvent, &App::on_set_object_visibility>();
  app.connect<SetObjectTagEvent, &App::on_set_object_tag>();
  app.connect<SpawnObjectContextMenuEvent, &App::on_spawn_object_context_menu>();

  app.connect<ShowAddPropertyDialogEvent, &App::on_show_add_property_dialog>();
  app.connect<ShowRenamePropertyDialogEvent, &App::on_show_rename_property_dialog>();
  app.connect<ShowChangePropertyTypeDialogEvent,
              &App::on_show_change_property_type_dialog>();
  app.connect<AddPropertyEvent, &App::on_add_property>();
  app.connect<RemovePropertyEvent, &App::on_remove_property>();
  app.connect<RenamePropertyEvent, &App::on_rename_property>();
  app.connect<UpdatePropertyEvent, &App::on_update_property>();
  app.connect<ChangePropertyTypeEvent, &App::on_change_property_type>();
  app.connect<InspectContextEvent, &App::on_inspect_context>();

  app.connect<OpenComponentEditorEvent, &App::on_open_component_editor>();
  app.connect<CreateComponentDefEvent, &App::on_create_component_def>();
  app.connect<RemoveComponentDefEvent, &App::on_remove_component_def>();
  app.connect<RenameComponentDefEvent, &App::on_rename_component_def>();
  app.connect<CreateComponentAttrEvent, &App::on_create_component_attr>();
  app.connect<RemoveComponentAttrEvent, &App::on_remove_component_attr>();
  app.connect<RenameComponentAttrEvent, &App::on_rename_component_attr>();
  app.connect<DuplicateComponentAttrEvent, &App::on_duplicate_component_attr>();
  app.connect<SetComponentAttrTypeEvent, &App::on_set_component_attr_type>();
  app.connect<UpdateComponentDefAttrEvent, &App::on_update_component_def_attr>();

  app.connect<AddComponentEvent, &App::on_add_component>();
  app.connect<RemoveComponentEvent, &App::on_remove_component>();
  app.connect<UpdateComponentEvent, &App::on_update_component>();
  app.connect<ResetComponentValuesEvent, &App::on_reset_component_values>();

  app.connect<ToggleUiEvent, &App::on_toggle_ui>();
  app.connect<QuitEvent, &App::on_quit>();
}

}  // namespace tactile
