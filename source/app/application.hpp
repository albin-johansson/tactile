#pragma once

#include <centurion.hpp>
#include <entt/entt.hpp>

#include "core/utils/texture_manager.hpp"
#include "editor/events/command_events.hpp"
#include "editor/events/component_events.hpp"
#include "editor/events/layer_events.hpp"
#include "editor/events/map_events.hpp"
#include "editor/events/object_events.hpp"
#include "editor/events/property_events.hpp"
#include "editor/events/quit_event.hpp"
#include "editor/events/save_events.hpp"
#include "editor/events/tileset_events.hpp"
#include "editor/events/tool_events.hpp"
#include "editor/events/viewport_events.hpp"
#include "editor/gui/icons.hpp"
#include "editor/gui/widget_manager.hpp"
#include "editor/model.hpp"
#include "tactile_def.hpp"

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
class application final {
  friend void subscribe_to_events(application&);

 public:
  explicit application(app_configuration* configuration);

  auto run() -> int;

 private:
  app_configuration* mConfiguration{}; /* Non-owning */
  entt::dispatcher mDispatcher;
  cen::keyboard mKeyboard;
  Model mModel;
  texture_manager mTextures;
  Icons mIcons;
  WidgetManager mWidgets;
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
  void on_set_command_capacity(const SetCommandCapacityEvent& event);

  void on_save();
  void on_save_as(const SaveAsEvent& event);
  void on_open_save_as_dialog();
  void on_show_settings();

  void on_show_new_map_dialog();
  void on_show_open_map_dialog();
  void on_show_map_properties();
  void on_create_map(const CreateMapEvent& event);
  void on_close_map(const CloseMapEvent& event);
  void on_open_map(const OpenMapEvent& event);
  void on_select_map(const SelectMapEvent& event);

  void on_select_tool(const SelectToolEvent& event);
  void on_mouse_pressed(const MousePressedEvent& event);
  void on_mouse_drag(const MouseDragEvent& event);
  void on_mouse_released(const MouseReleasedEvent& event);
  void on_stamp_sequence(StampSequenceEvent event);
  void on_eraser_sequence(EraserSequenceEvent event);
  void on_flood(const FloodEvent& event);

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
  void on_add_tileset(const AddTilesetEvent& event);
  void on_remove_tileset(const RemoveTilesetEvent& event);
  void on_select_tileset(const SelectTilesetEvent& event);
  void on_set_tileset_selection(const SetTilesetSelectionEvent& event);
  void on_set_tileset_name(const SetTilesetNameEvent& event);

  void on_add_row();
  void on_add_column();
  void on_remove_row();
  void on_remove_column();
  void on_resize_map(const ResizeMapEvent& event);
  void on_open_resize_map_dialog();

  void on_add_layer(const AddLayerEvent& event);
  void on_remove_layer(const RemoveLayerEvent& event);
  void on_select_layer(const SelectLayerEvent& event);
  void on_move_layer_up(const MoveLayerUpEvent& event);
  void on_move_layer_down(const MoveLayerDownEvent& event);
  void on_duplicate_layer(const DuplicateLayerEvent& event);
  void on_set_layer_opacity(const SetLayerOpacityEvent& event);
  void on_set_layer_visible(const SetLayerVisibleEvent& event);
  void on_open_rename_layer_dialog(const OpenRenameLayerDialogEvent& event);
  void on_rename_layer(const RenameLayerEvent& event);

  void on_set_object_name(const SetObjectNameEvent& event);
  void on_move_object(const MoveObjectEvent& event);
  void on_set_object_visibility(const SetObjectVisibilityEvent& event);
  void on_set_object_tag(const SetObjectTagEvent& event);
  void on_spawn_object_context_menu(const SpawnObjectContextMenuEvent& event);

  void on_show_add_property_dialog();
  void on_show_rename_property_dialog(const ShowRenamePropertyDialogEvent& event);
  void on_show_change_property_type_dialog(
      const ShowChangePropertyTypeDialogEvent& event);
  void on_add_property(const AddPropertyEvent& event);
  void on_remove_property(const RemovePropertyEvent& event);
  void on_rename_property(const RenamePropertyEvent& event);
  void on_update_property(const UpdatePropertyEvent& event);
  void on_change_property_type(const ChangePropertyTypeEvent& event);
  void on_inspect_context(const InspectContextEvent& event);

  void on_open_component_editor();
  void on_create_component_def(const CreateComponentDefEvent& event);
  void on_remove_component_def(const RemoveComponentDefEvent& event);
  void on_rename_component_def(const RenameComponentDefEvent& event);
  void on_create_component_attribute(const CreateComponentAttributeEvent& event);
  void on_remove_component_attribute(const RemoveComponentAttributeEvent& event);
  void on_rename_component_attribute(const RenameComponentAttributeEvent& event);
  void on_duplicate_component_attribute(const DuplicateComponentAttributeEvent& event);
  void on_set_component_attribute_type(const SetComponentAttributeTypeEvent& event);
  void on_update_component_def_attribute(const UpdateComponentDefAttributeEvent& event);

  void on_add_component(const AddComponentEvent& event);
  void on_remove_component(const RemoveComponentEvent& event);
  void on_update_component(const UpdateComponentEvent& event);
  void on_reset_component_values(const ResetComponentValuesEvent& event);

  void on_toggle_ui();
  void on_quit();
};

}  // namespace tactile
