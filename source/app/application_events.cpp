#include "application_events.hpp"

#include "application.hpp"
#include "editor/events/edit_events.hpp"
#include "editor/events/map_events.hpp"
#include "editor/events/tileset_events.hpp"
#include "editor/events/view_events.hpp"
#include "editor/events/viewport_events.hpp"

namespace tactile {

using appl = application;

void subscribe_to_events(appl& app)
{
  app.connect<UndoEvent, &appl::on_undo>();
  app.connect<RedoEvent, &appl::on_redo>();
  app.connect<SetCommandCapacityEvent, &appl::on_set_command_capacity>();

  app.connect<SaveEvent, &appl::on_save>();
  app.connect<SaveAsEvent, &appl::on_save_as>();
  app.connect<OpenSaveAsDialogEvent, &appl::on_open_save_as_dialog>();
  app.connect<ShowSettingsEvent, &appl::on_show_settings>();

  app.connect<ShowNewMapDialogEvent, &appl::on_show_new_map_dialog>();
  app.connect<ShowOpenMapDialogEvent, &appl::on_show_open_map_dialog>();
  app.connect<ShowMapPropertiesEvent, &appl::on_show_map_properties>();
  app.connect<CreateMapEvent, &appl::on_create_map>();
  app.connect<CloseMapEvent, &appl::on_close_map>();
  app.connect<OpenMapEvent, &appl::on_open_map>();
  app.connect<SelectMapEvent, &appl::on_select_map>();

  app.connect<SelectToolEvent, &appl::on_select_tool>();
  app.connect<MousePressedEvent, &appl::on_mouse_pressed>();
  app.connect<MouseDragEvent, &appl::on_mouse_drag>();
  app.connect<MouseReleasedEvent, &appl::on_mouse_released>();
  app.connect<StampSequenceEvent, &appl::on_stamp_sequence>();
  app.connect<EraserSequenceEvent, &appl::on_eraser_sequence>();
  app.connect<FloodEvent, &appl::on_flood>();

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

  app.connect<ShowAddTilesetDialogEvent, &appl::on_show_add_tileset_dialog>();
  app.connect<AddTilesetEvent, &appl::on_add_tileset>();
  app.connect<RemoveTilesetEvent, &appl::on_remove_tileset>();
  app.connect<SelectTilesetEvent, &appl::on_select_tileset>();
  app.connect<SetTilesetSelectionEvent, &appl::on_set_tileset_selection>();
  app.connect<SetTilesetNameEvent, &appl::on_set_tileset_name>();

  app.connect<AddRowEvent, &appl::on_add_row>();
  app.connect<AddColumnEvent, &appl::on_add_column>();
  app.connect<RemoveRowEvent, &appl::on_remove_row>();
  app.connect<RemoveColumnEvent, &appl::on_remove_column>();
  app.connect<ResizeMapEvent, &appl::on_resize_map>();
  app.connect<OpenResizeMapDialogEvent, &appl::on_open_resize_map_dialog>();

  app.connect<AddLayerEvent, &appl::on_add_layer>();
  app.connect<RemoveLayerEvent, &appl::on_remove_layer>();
  app.connect<SelectLayerEvent, &appl::on_select_layer>();
  app.connect<MoveLayerUpEvent, &appl::on_move_layer_up>();
  app.connect<MoveLayerDownEvent, &appl::on_move_layer_down>();
  app.connect<DuplicateLayerEvent, &appl::on_duplicate_layer>();
  app.connect<SetLayerOpacityEvent, &appl::on_set_layer_opacity>();
  app.connect<SetLayerVisibleEvent, &appl::on_set_layer_visible>();
  app.connect<OpenRenameLayerDialogEvent, &appl::on_open_rename_layer_dialog>();
  app.connect<RenameLayerEvent, &appl::on_rename_layer>();

  app.connect<SetObjectNameEvent, &appl::on_set_object_name>();
  app.connect<MoveObjectEvent, &appl::on_move_object>();
  app.connect<SetObjectVisibilityEvent, &appl::on_set_object_visibility>();
  app.connect<SetObjectTagEvent, &appl::on_set_object_tag>();
  app.connect<SpawnObjectContextMenuEvent, &appl::on_spawn_object_context_menu>();

  app.connect<ShowAddPropertyDialogEvent, &appl::on_show_add_property_dialog>();
  app.connect<ShowRenamePropertyDialogEvent, &appl::on_show_rename_property_dialog>();
  app.connect<ShowChangePropertyTypeDialogEvent,
              &appl::on_show_change_property_type_dialog>();
  app.connect<AddPropertyEvent, &appl::on_add_property>();
  app.connect<RemovePropertyEvent, &appl::on_remove_property>();
  app.connect<RenamePropertyEvent, &appl::on_rename_property>();
  app.connect<UpdatePropertyEvent, &appl::on_update_property>();
  app.connect<ChangePropertyTypeEvent, &appl::on_change_property_type>();
  app.connect<InspectContextEvent, &appl::on_inspect_context>();

  app.connect<OpenComponentEditorEvent, &appl::on_open_component_editor>();
  app.connect<CreateComponentDefEvent, &appl::on_create_component_def>();
  app.connect<RemoveComponentDefEvent, &appl::on_remove_component_def>();
  app.connect<RenameComponentDefEvent, &appl::on_rename_component_def>();
  app.connect<CreateComponentAttributeEvent, &appl::on_create_component_attribute>();
  app.connect<RemoveComponentAttributeEvent, &appl::on_remove_component_attribute>();
  app.connect<RenameComponentAttributeEvent, &appl::on_rename_component_attribute>();
  app.connect<DuplicateComponentAttributeEvent,
              &appl::on_duplicate_component_attribute>();
  app.connect<SetComponentAttributeTypeEvent, &appl::on_set_component_attribute_type>();
  app.connect<UpdateComponentDefAttributeEvent,
              &appl::on_update_component_def_attribute>();

  app.connect<AddComponentEvent, &appl::on_add_component>();
  app.connect<RemoveComponentEvent, &appl::on_remove_component>();
  app.connect<UpdateComponentEvent, &appl::on_update_component>();
  app.connect<ResetComponentValuesEvent, &appl::on_reset_component_values>();

  app.connect<ToggleUiEvent, &appl::on_toggle_ui>();
  app.connect<QuitEvent, &appl::on_quit>();
}

}  // namespace tactile
