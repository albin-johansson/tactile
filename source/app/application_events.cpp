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

#include <entt/signal/dispatcher.hpp>

#include "application.hpp"
#include "core/events/map_events.hpp"
#include "core/events/misc_events.hpp"
#include "core/events/tileset_events.hpp"
#include "core/events/viewport_events.hpp"
#include "editor/gui/menus/edit_menu.hpp"
#include "editor/gui/menus/file_menu.hpp"
#include "editor/gui/menus/map_menu.hpp"
#include "editor/gui/properties/property_dock.hpp"
#include "editor/gui/viewport/map_view.hpp"

namespace tactile {

using App = Application;

void subscribe_to_events(App* app)
{
  auto& d = app->get_dispatcher();

  d.sink<UndoEvent>().connect<&App::on_undo>(app);
  d.sink<RedoEvent>().connect<&App::on_redo>(app);
  d.sink<SetCommandCapacityEvent>().connect<&App::on_set_command_capacity>(app);

  d.sink<SaveEvent>().connect<&App::on_save>(app);
  d.sink<SaveAsEvent>().connect<&App::on_save_as>(app);
  d.sink<OpenSaveAsDialogEvent>().connect<&App::on_open_save_as_dialog>(app);
  d.sink<ShowSettingsEvent>().connect<&show_settings_dialog>();

  d.sink<ShowNewMapDialogEvent>().connect<&show_map_creation_dialog>();
  d.sink<ShowOpenMapDialogEvent>().connect<&show_map_selector_dialog>();
  d.sink<InspectMapEvent>().connect<&App::on_show_map_properties>(app);
  d.sink<CreateMapEvent>().connect<&App::on_create_map>(app);
  d.sink<CloseMapEvent>().connect<&App::on_close_map>(app);
  d.sink<OpenMapEvent>().connect<&App::on_open_map>(app);
  d.sink<SelectMapEvent>().connect<&App::on_select_map>(app);

  d.sink<SelectToolEvent>().connect<&App::on_select_tool>(app);
  d.sink<ToolPressedEvent>().connect<&App::on_tool_pressed>(app);
  d.sink<ToolDraggedEvent>().connect<&App::on_tool_dragged>(app);
  d.sink<ToolReleasedEvent>().connect<&App::on_tool_released>(app);
  d.sink<ToolEnteredEvent>().connect<&App::on_tool_entered>(app);
  d.sink<ToolExitedEvent>().connect<&App::on_tool_exited>(app);

  d.sink<StampSequenceEvent>().connect<&App::on_stamp_sequence>(app);
  d.sink<SetStampRandomizerEvent>().connect<&App::on_set_stamp_randomizer_event>(app);
  d.sink<EraserSequenceEvent>().connect<&App::on_eraser_sequence>(app);
  d.sink<FloodEvent>().connect<&App::on_flood>(app);
  d.sink<AddRectangleEvent>().connect<&App::on_add_rectangle>(app);
  d.sink<AddEllipseEvent>().connect<&App::on_add_ellipse>(app);
  d.sink<AddPointEvent>().connect<&App::on_add_point>(app);

  d.sink<CenterViewportEvent>().connect<&center_map_viewport>();
  d.sink<OffsetViewportEvent>().connect<&App::on_offset_viewport>(app);
  d.sink<OffsetBoundViewportEvent>().connect<&App::on_offset_bound_viewport>(app);
  d.sink<PanLeftEvent>().connect<&App::on_pan_left>(app);
  d.sink<PanRightEvent>().connect<&App::on_pan_right>(app);
  d.sink<PanUpEvent>().connect<&App::on_pan_up>(app);
  d.sink<PanDownEvent>().connect<&App::on_pan_down>(app);
  d.sink<IncreaseZoomEvent>().connect<&App::on_increase_zoom>(app);
  d.sink<DecreaseZoomEvent>().connect<&App::on_decrease_zoom>(app);
  d.sink<ResetZoomEvent>().connect<&App::on_reset_zoom>(app);
  d.sink<ResetFontSizeEvent>().connect<&App::on_reset_font_size>(app);
  d.sink<IncreaseFontSizeEvent>().connect<&App::on_increase_font_size>(app);
  d.sink<DecreaseFontSizeEvent>().connect<&App::on_decrease_font_size>(app);

  d.sink<ShowTilesetCreationDialogEvent>().connect<&show_tileset_creation_dialog>();
  d.sink<AddTilesetEvent>().connect<&App::on_add_tileset>(app);
  d.sink<RemoveTilesetEvent>().connect<&App::on_remove_tileset>(app);
  d.sink<SelectTilesetEvent>().connect<&App::on_select_tileset>(app);
  d.sink<SetTilesetSelectionEvent>().connect<&App::on_set_tileset_selection>(app);
  d.sink<SetTilesetNameEvent>().connect<&App::on_set_tileset_name>(app);

  d.sink<AddRowEvent>().connect<&App::on_add_row>(app);
  d.sink<AddColumnEvent>().connect<&App::on_add_column>(app);
  d.sink<RemoveRowEvent>().connect<&App::on_remove_row>(app);
  d.sink<RemoveColumnEvent>().connect<&App::on_remove_column>(app);
  d.sink<ResizeMapEvent>().connect<&App::on_resize_map>(app);
  d.sink<OpenResizeMapDialogEvent>().connect<&App::on_open_resize_map_dialog>(app);

  d.sink<AddLayerEvent>().connect<&App::on_add_layer>(app);
  d.sink<RemoveLayerEvent>().connect<&App::on_remove_layer>(app);
  d.sink<SelectLayerEvent>().connect<&App::on_select_layer>(app);
  d.sink<MoveLayerUpEvent>().connect<&App::on_move_layer_up>(app);
  d.sink<MoveLayerDownEvent>().connect<&App::on_move_layer_down>(app);
  d.sink<DuplicateLayerEvent>().connect<&App::on_duplicate_layer>(app);
  d.sink<SetLayerOpacityEvent>().connect<&App::on_set_layer_opacity>(app);
  d.sink<SetLayerVisibleEvent>().connect<&App::on_set_layer_visible>(app);
  d.sink<OpenRenameLayerDialogEvent>().connect<&App::on_open_rename_layer_dialog>(app);
  d.sink<RenameLayerEvent>().connect<&App::on_rename_layer>(app);

  d.sink<SetObjectNameEvent>().connect<&App::on_set_object_name>(app);
  d.sink<MoveObjectEvent>().connect<&App::on_move_object>(app);
  d.sink<SetObjectVisibilityEvent>().connect<&App::on_set_object_visibility>(app);
  d.sink<SetObjectTagEvent>().connect<&App::on_set_object_tag>(app);
  d.sink<SpawnObjectContextMenuEvent>().connect<&App::on_spawn_object_context_menu>(app);

  d.sink<ShowAddPropertyDialogEvent>().connect<&show_property_creation_dialog>();
  d.sink<ShowRenamePropertyDialogEvent>().connect<&App::on_show_rename_property_dialog>(
      app);
  d.sink<ShowChangePropertyTypeDialogEvent>()
      .connect<&App::on_show_change_property_type_dialog>(app);
  d.sink<AddPropertyEvent>().connect<&App::on_add_property>(app);
  d.sink<RemovePropertyEvent>().connect<&App::on_remove_property>(app);
  d.sink<RenamePropertyEvent>().connect<&App::on_rename_property>(app);
  d.sink<UpdatePropertyEvent>().connect<&App::on_update_property>(app);
  d.sink<ChangePropertyTypeEvent>().connect<&App::on_change_property_type>(app);
  d.sink<InspectContextEvent>().connect<&App::on_inspect_context>(app);

  d.sink<OpenComponentEditorEvent>().connect<&App::on_open_component_editor>(app);
  d.sink<CreateComponentDefEvent>().connect<&App::on_create_component_def>(app);
  d.sink<RemoveComponentDefEvent>().connect<&App::on_remove_component_def>(app);
  d.sink<RenameComponentDefEvent>().connect<&App::on_rename_component_def>(app);
  d.sink<CreateComponentAttrEvent>().connect<&App::on_create_component_attr>(app);
  d.sink<RemoveComponentAttrEvent>().connect<&App::on_remove_component_attr>(app);
  d.sink<RenameComponentAttrEvent>().connect<&App::on_rename_component_attr>(app);
  d.sink<DuplicateComponentAttrEvent>().connect<&App::on_duplicate_component_attr>(app);
  d.sink<SetComponentAttrTypeEvent>().connect<&App::on_set_component_attr_type>(app);
  d.sink<UpdateComponentDefAttrEvent>().connect<&App::on_update_component_def_attr>(app);

  d.sink<AddComponentEvent>().connect<&App::on_add_component>(app);
  d.sink<RemoveComponentEvent>().connect<&App::on_remove_component>(app);
  d.sink<UpdateComponentEvent>().connect<&App::on_update_component>(app);
  d.sink<ResetComponentValuesEvent>().connect<&App::on_reset_component_values>(app);

  d.sink<ToggleUiEvent>().connect<&App::on_toggle_ui>(app);
  d.sink<ReloadFontsEvent>().connect<&App::on_reload_fonts>(app);
  d.sink<QuitEvent>().connect<&App::on_quit>(app);
}

}  // namespace tactile
